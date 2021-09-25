#include <jni.h>
#include <string>
#include <unordered_map>
#include <hwcpipe.h>
#include <mutex>

hwcpipe::HWCPipe *h;
std::mutex lock; // serve only one at a time
std::unordered_map<hwcpipe::CpuCounter, std::string> CpuCounterNames;
std::unordered_map<hwcpipe::GpuCounter, std::string> GpuCounterNames;
std::unordered_map<hwcpipe::CpuCounter, double> CpuCounterValueTotal;
std::unordered_map<hwcpipe::GpuCounter, double> GpuCounterValueTotal;

extern "C"
JNIEXPORT void JNICALL
Java_at_ylz_hwcpipe_1exporter_HWCPipeExports_start(JNIEnv *env, jobject thiz) {
    CpuCounterNames = {
            {hwcpipe::CpuCounter::Cycles,             "cycles"},
            {hwcpipe::CpuCounter::Instructions,       "instructions"},
            {hwcpipe::CpuCounter::CacheReferences,    "cache_references"},
            {hwcpipe::CpuCounter::CacheMisses,        "cache_misses"},
            {hwcpipe::CpuCounter::BranchInstructions, "branch_instructions"},
            {hwcpipe::CpuCounter::BranchMisses,       "branch_misses"},

            {hwcpipe::CpuCounter::L1Accesses,         "l1_accesses"},
            {hwcpipe::CpuCounter::InstrRetired,       "instr_retired"},
            {hwcpipe::CpuCounter::L2Accesses,         "l2_accesses"},
            {hwcpipe::CpuCounter::L3Accesses,         "l3_accesses"},
            {hwcpipe::CpuCounter::BusReads,           "bus_reads"},
            {hwcpipe::CpuCounter::BusWrites,          "bus_writes"},
            {hwcpipe::CpuCounter::MemReads,           "mem_reads"},
            {hwcpipe::CpuCounter::MemWrites,          "mem_writes"},
            {hwcpipe::CpuCounter::ASESpec,            "ase_spec"},
            {hwcpipe::CpuCounter::VFPSpec,            "vfp_spec"},
            {hwcpipe::CpuCounter::CryptoSpec,         "crypto_spec"},
    };

    GpuCounterNames = {
            {hwcpipe::GpuCounter::GpuCycles,                   "mali_gpu_active_cy"},
            {hwcpipe::GpuCounter::VertexComputeCycles,         "mali_non_frag_queue_active_cy"},
            {hwcpipe::GpuCounter::FragmentCycles,              "mali_frag_queue_active_cy"},
            {hwcpipe::GpuCounter::TilerCycles,                 "mali_tiler_active_cy"},

            {hwcpipe::GpuCounter::VertexComputeJobs,           "mali_non_frag_queue_job"},
            {hwcpipe::GpuCounter::Tiles,                       "mali_frag_tile"},
            {hwcpipe::GpuCounter::TransactionEliminations,     "mali_frag_tile_kill"},
            {hwcpipe::GpuCounter::FragmentJobs,                "mali_frag_queue_job"},
            {hwcpipe::GpuCounter::Pixels,                      "mali_gpu_pix"},

            {hwcpipe::GpuCounter::EarlyZTests,                 "mali_frag_ezs_test_qd"},
            {hwcpipe::GpuCounter::EarlyZKilled,                "mali_frag_ezs_kill_qd"},
            {hwcpipe::GpuCounter::LateZTests,                  "mali_frag_lzs_test_qd"},
            {hwcpipe::GpuCounter::LateZKilled,                 "mali_frag_lzs_kill_qd"},

            {hwcpipe::GpuCounter::Instructions,                "mali_eng_instr"},
            {hwcpipe::GpuCounter::DivergedInstructions,        "mali_eng_diverged_instr"},

            {hwcpipe::GpuCounter::ShaderCycles,                "mali_core_active_cy"},
            {hwcpipe::GpuCounter::ShaderArithmeticCycles,      "mali_eng_instr_shader_arithmetic_cycles"}, // this is identical with GpuCounter::Instructions
            {hwcpipe::GpuCounter::ShaderLoadStoreCycles,       "mali_ls_issue_cy"},
            {hwcpipe::GpuCounter::ShaderTextureCycles,         "mali_tex_filt_active_cy"},

            {hwcpipe::GpuCounter::CacheReadLookups,            "mali_l2cache_rd_lookup"},
            {hwcpipe::GpuCounter::CacheWriteLookups,           "mali_l2cache_wr_lookup"},
            {hwcpipe::GpuCounter::ExternalMemoryReadAccesses,  "mali_ext_bus_rd"},
            {hwcpipe::GpuCounter::ExternalMemoryWriteAccesses, "mali_ext_bus_wr"},
            {hwcpipe::GpuCounter::ExternalMemoryReadStalls,    "mali_ext_bus_rd_stall_cy"},
            {hwcpipe::GpuCounter::ExternalMemoryWriteStalls,   "mali_ext_bus_wr_stall_cy"},
            {hwcpipe::GpuCounter::ExternalMemoryReadBytes,     "mali_ext_bus_rd_by"},
            {hwcpipe::GpuCounter::ExternalMemoryWriteBytes,    "mali_ext_bus_wr_by"},
    };


    h = new hwcpipe::HWCPipe();
    auto cpu_counters = h->cpu_profiler()->supported_counters();
    for (auto counter : cpu_counters) {
        auto counter_name = "cpu_" + CpuCounterNames[counter];
        CpuCounterNames[counter] = counter_name + "#" +
                                   hwcpipe::cpu_counter_info.at(counter).desc + "#" +
                                   hwcpipe::cpu_counter_info.at(counter).unit;
        CpuCounterValueTotal[counter] = 0.0;
    }
    auto gpu_counters = h->gpu_profiler()->supported_counters();
    for (auto counter : gpu_counters) {
        auto counter_name = "gpu_" + GpuCounterNames[counter];
        GpuCounterNames[counter] = counter_name + "#" +
                                   hwcpipe::gpu_counter_info.at(counter).desc + "#" +
                                   hwcpipe::gpu_counter_info.at(counter).unit;
        GpuCounterValueTotal[counter] = 0.0;
    }

    h->run();
}

extern "C"
JNIEXPORT void JNICALL
Java_at_ylz_hwcpipe_1exporter_HWCPipeExports_stop(JNIEnv *env, jobject thiz) {
    h->stop();
    delete h;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_at_ylz_hwcpipe_1exporter_HWCPipeExports_sample(JNIEnv *env, jobject thiz) {
    lock.lock();
    auto measurements = h->sample();
    jclass java_cls_HashMap = env->FindClass("java/util/HashMap");
    jclass java_cls_Double = env->FindClass("java/lang/Double");
    jmethodID java_mid_HashMap_init = env->GetMethodID(java_cls_HashMap, "<init>", "()V");
    jmethodID java_mid_Double_init = env->GetMethodID(java_cls_Double, "<init>", "(D)V");
    jobject java_obj_HashMap = env->NewObject(java_cls_HashMap, java_mid_HashMap_init);
    jmethodID java_mid_HashMap_put = env->GetMethodID(java_cls_HashMap, "put",
                                                      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (measurements.cpu) {
        auto counters = h->cpu_profiler()->supported_counters();
        for (auto counter: counters) {
            const auto &counter_iterator = measurements.cpu->find(counter);
            if (counter_iterator != measurements.cpu->end()) {
                CpuCounterValueTotal[counter] += counter_iterator->second.get<double>();
                double current_val = CpuCounterValueTotal[counter];
                jobject java_obj_Double = env->NewObject(java_cls_Double,
                                                         java_mid_Double_init,
                                                         current_val);
                env->CallObjectMethod(java_obj_HashMap, java_mid_HashMap_put,
                                      env->NewStringUTF(CpuCounterNames[counter].c_str()),
                                      java_obj_Double);
            }
        }
    }
    if (measurements.gpu) {
        auto counters = h->gpu_profiler()->supported_counters();
        for (auto counter: counters) {
            const auto &counter_iterator = measurements.gpu->find(counter);
            if (counter_iterator != measurements.gpu->end()) {
                GpuCounterValueTotal[counter] += counter_iterator->second.get<double>();
                double current_val = GpuCounterValueTotal[counter];
                jobject java_obj_Double = env->NewObject(java_cls_Double,
                                                         java_mid_Double_init,
                                                         current_val);
                env->CallObjectMethod(java_obj_HashMap, java_mid_HashMap_put,
                                      env->NewStringUTF(GpuCounterNames[counter].c_str()),
                                      java_obj_Double);
            }
        }
    }
    lock.unlock();
    return java_obj_HashMap;
}