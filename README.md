# ARM® HWCPipe Exporter

ARM® HWCPipe Exporter is a [Prometheus exporter](https://prometheus.io/docs/instrumenting/exporters/) written in Java and C++ that retrieves metrics from Android devices running on ARM® Hardware components and exports them to the [Prometheus](https://prometheus.io/) monitoring system.

## Quickstart

- Install HWCPipe Exporter

  ```shell
  $ adb install -t at.ylz.hwcpipe_exporter.apk
  ```

- Run the exporter as follows

  ```shell
  $ adb shell am start -n at.ylz.hwcpipe_exporter/at.ylz.hwcpipe_exporter.MainActivity
  ```

- Find your Android device's IP

- Install and configure Prometheus

  An example of [Prometheus configuration](https://prometheus.io/docs/prometheus/latest/configuration/configuration/) file named prometheus.yml can be as simple as follows

  ```yaml
  global:
    scrape_interval: 15s # By default, scrape targets every 15 seconds.
  
    # Attach these labels to any time series or alerts when communicating with
    # external systems (federation, remote storage, Alertmanager).
    external_labels:
      monitor: 'hwcpipe-exporter-monitor'
  
  # A scrape configuration containing exactly one endpoint to scrape:
  # Here it's Prometheus itself.
  scrape_configs:
    # The job name is added as a label `job=<job_name>` to any timeseries scraped from this config.
    - job_name: 'hwcpipe-exporter'
  
      # Override the global default and scrape targets from this job every 5 seconds.
      scrape_interval: 2s
  
      static_configs:
        - targets: [ '<your-android-device-ip>:9998' ]
  ```

  > For overview of Prometheus, please see [Prometheus Overview](https://prometheus.io/docs/introduction/overview/).
  >
  > For installation of Prometheus, Please see [Prometheus Installation Guide](https://prometheus.io/docs/prometheus/latest/installation/).

- Direct your web browser at [http://localhost:9090](http://localhost:9090) to see the exported Prometheus metrics

## ARM® Counters

| Metrics | HWCPipe | ARM counter | Description | Unit |
| :---- | :---- | :---- | :---- | :---- |
|  hwcpipe_cpu_cycles | CpuCounter::Cycles |  Cycles | Number of CPU cycles | cycles |
|  hwcpipe_cpu_instructions | CpuCounter::Instructions |  Instructions | Number of CPU instructions | instructions |
|  hwcpipe_cpu_cache_references | CpuCounter::CacheReferences |  CacheReferences | Number of cache references | references |
|  hwcpipe_cpu_cache_misses | CpuCounter::CacheMisses |  CacheMisses | Number of cache misses | misses |
|  hwcpipe_cpu_branch_instructions | CpuCounter::BranchInstructions |  BranchInstructions | Number of branch instructionss | instructions |
|  hwcpipe_cpu_branch_misses | CpuCounter::BranchMisses |  BranchMisses | Number of branch misses | misses |
|  hwcpipe_cpu_l1_accesses | CpuCounter::L1Accesses |  L1Accesses | L1 data cache accesses | accesses |
|  hwcpipe_cpu_instr_retired | CpuCounter::InstrRetired |  InstrRetired | All retired instructions | instructions |
|  hwcpipe_cpu_l2_accesses | CpuCounter::L2Accesses |  L2Accesses | L2 data cache accesses | accesses |
|  hwcpipe_cpu_l3_accesses | CpuCounter::L3Accesses |  L3Accesses | L3 data cache accesses | accesses |
|  hwcpipe_cpu_bus_reads | CpuCounter::BusReads |  BusReads | Bus access reads | beats |
|  hwcpipe_cpu_bus_writes | CpuCounter::BusWrites |  BusWrites | Bus access writes | beats |
|  hwcpipe_cpu_mem_reads | CpuCounter::MemReads |  MemReads | Data memory access, load instructions |instructions |
|  hwcpipe_cpu_mem_writes | CpuCounter::MemWrites |  MemWrites | Data memory access, store instructions | instructions |
|  hwcpipe_cpu_ase_spec | CpuCounter::ASESpec |  ASESpec | Speculatively executed SIMD operations | operations |
|  hwcpipe_cpu_vfp_spec | CpuCounter::VFPSpec |  VFPSpec | Speculatively executed floating point operations | operations |
|  hwcpipe_cpu_crypto_spec | CpuCounter::CryptoSpec |  CryptoSpec | Speculatively executed cryptographic operations | operations |
| hwcpipe_gpu_mali_gpu_active_cy | GpuCounter::GpuCycles | MaliGPUActiveCy | GPU active cycles. This counter increments every clock cycle where the GPU has any pending workload present in one of its processing queues, and therefore shows the overall GPU processing load requested by the application. This counter will increment every clock cycle where any workload is present in a processing queue, even if the GPU is stalled waiting for external memory to return data; this is still counted as active time even though no forward progress is being made. Hardware name: GPU_ACTIVE. | cycles |
| hwcpipe_gpu_mali_non_frag_queue_active_cy |GpuCounter::VertexComputeCycles | MaliNonFragQueueActiveCy | Non-fragment queue active cycles. This counter increments every clock cycle where the GPU has any workload present in the non-fragment queue. This queue can be used for vertex shaders, tessellation shaders, geometry shaders, fixed function tiling, and compute shaders. This counter can not disambiguate between these workloads. This counter will increment any clock cycle where a workload is loaded into a queue even if the GPU is stalled waiting for external memory to return data; this is still counted as active time even though no forward progress is being made. Hardware name: JS1_ACTIVE. | cycles |
| hwcpipe_gpu_mali_frag_queue_active_cy | GpuCounter::FragmentCycles | MaliFragQueueActiveCy | Fragment queue active cycles. This counter increments every clock cycle where the GPU has any workload present in the fragment queue. For most graphics content there are significantly more fragments than vertices, so this queue will normally have the highest processing load. In content that is GPU bound by fragment processing it is normal for to be approximately equal to , with vertex and fragment processing running in parallel. This counter will increment any clock cycle where a workload is loaded into a queue even if the GPU is stalled waiting for external memory to return data; this is still counted as active time even though no forward progress is being made. Hardware name: JS0_ACTIVE. | cycles |
| hwcpipe_gpu_mali_tiler_active_cy | GpuCounter::TilerCycles | MaliTilerActiveCy | Tiler active cycles. This counter increments every cycle the tiler has a workload in its processing queue. The tiler can run in parallel to vertex shading and fragment shading so a high cycle count here does not necessarily imply a bottleneck, unless the counters in the shader cores are very low relative to this. Hardware name: TILER_ACTIVE. | cycles |
| hwcpipe_gpu_mali_non_frag_queue_job | GpuCounter::VertexComputeJobs | MaliNonFragQueueJob  | Non-fragment jobs. This counter increments for every job processed by the GPU non-fragment queue. Hardware name: JS1_JOBS. | jobs |
| hwcpipe_gpu_mali_frag_tile | GpuCounter::Tiles | MaliFragTile | Tiles. This counter increments for every tile processed by the shader core. Note that tiles are normally 16x16 pixels but can vary depending on per-pixel storage requirements and the tile buffer size of the current GPU. The number of bits of color storage per pixel available when using a 16x16 tile size on this GPU is 256. Using more storage than this - whether for multi-sampling, wide color formats, or multiple render targets - will result in the driver dynamically reducing tile size until sufficient storage is available. The most accurate way to get the total pixel count rendered by the application is to use the job manager counter, because it will always count 32x32 pixel regions. Hardware name: FRAG_PTILES. | tiles |
| hwcpipe_gpu_mali_frag_tile_kill | GpuCounter::TransactionEliminations | MaliFragTileKill | Constant tiles killed. This counter increments for every tile killed by a transaction elimination CRC check. Hardware name: FRAG_TRANS_ELIM. | tiles |
| hwcpipe_gpu_mali_frag_queue_job |GpuCounter::FragmentJobs | MaliFragQueueJob | Fragment jobs. This counter increments for every job processed by the GPU fragment queue. Hardware name: JS0_JOBS. | jobs |
| hwcpipe_gpu_mali_gpu_pix | GpuCounter::Pixels | MaliGPUPix | Pixels. This expression defines the total number of pixels that are shaded for any render pass. Note that this can be a slight overestimate because the underlying hardware counter rounds the width and height values of the rendered surface to be 32-pixel aligned, even if those pixels are not actually processed during shading because they are out of the active viewport and/or scissor region. | cycles |
| hwcpipe_gpu_mali_frag_ezs_test_qd | GpuCounter::EarlyZTests | MaliFragEZSTestQd | Early ZS tested quads. This counter increments for every quad undergoing early depth and stencil testing. For maximum performance, this number should be close to the total number of input quads. We want as many of the input quads as possible to be subject to early ZS testing as it is significantly more efficient than late ZS testing, which will only kill threads after they have been fragment shaded. Hardware name: FRAG_QUADS_EZS_TEST. | tests |
| hwcpipe_gpu_mali_frag_ezs_kill_qd | GpuCounter::EarlyZKilled | MaliFragEZSKillQd | Early ZS killed quads. This counter increments for every quad killed by early depth and stencil testing. It is common to see a proportion of quads killed at this point in the pipeline, because early ZS is effective at handling depth-based occlusion inside the view frustum, and can reduce the need for perfect culling in the application. However, if a very high percentage of quads are being killed at this stage, this can indicate that improvements in application culling are possible, such as the use of potential visibility sets or portal culling to cull objects in different rooms. Hardware name: FRAG_QUADS_EZS_KILL. | tests |
| hwcpipe_gpu_mali_frag_lzs_test_qd | GpuCounter::LateZTests  | MaliFragLZSTestQd | Late ZS tested quads. This counter increments for every quad undergoing late depth and stencil testing. Hardware name: FRAG_LZS_TEST. | tests |
| hwcpipe_gpu_mali_frag_lzs_kill_qd | GpuCounter::LateZKilled | MaliFragLZSKillQd | Late ZS killed quads. This counter increments for every quad killed by late depth and stencil testing. Hardware name: FRAG_LZS_KILL. | tests |
| hwcpipe_gpu_mali_eng_instr | GpuCounter::Instructions | MaliEngInstr | Executed instructions. This counter increments for every instruction that the execution engine processes per warp. All instructions are single cycle issue. Hardware name: EXEC_INSTR_COUNT. | instructions |
| hwcpipe_gpu_mali_eng_diverged_instr | GpuCounter::DivergedInstructions | MaliEngDivergedInstr | Diverged instructions. This counter increments for every instruction the execution engine processes per warp where there is control flow divergence across the warp. Control flow divergence erodes arithmetic execution efficiency because it implies some threads in the warp are idle because they did not take the current control path through the code. Aim to minimize control flow divergence when designing shader effects. Hardware name: EXEC_INSTR_DIVERGED. | instructions |
| hwcpipe_gpu_mali_core_active_cy | GpuCounter::ShaderCycles  | MaliCoreActiveCy | Execution core active cycles. This counter increments every cycle that the shader core is processing at least one warp. Note that this counter does not provide detailed information about how the functional units are utilized inside the shader core, but simply gives an indication that something was running. Hardware name: EXEC_CORE_ACTIVE. | cycles |
| hwcpipe_gpu_mali_eng_instr_shader_arithmetic_cycles | GpuCounter::ShaderArithmeticCycles | MaliEngInstr | Executed instructions. This counter increments for every instruction that the execution engine processes per warp. All instructions are single cycle issue. Hardware name: EXEC_INSTR_COUNT. | cycles |
| hwcpipe_gpu_mali_ls_issue_cy | GpuCounter::ShaderLoadStoreCycles | MaliLSIssueCy | Load/store total issues. This expression defines the total number of load/store issue cycles. Note that this counter ignores secondary effects such as cache misses, so this counter provides the best case cycle usage. | cycles |
| hwcpipe_gpu_mali_tex_filt_active_cy | GpuCounter::ShaderTextureCycles | MaliTexFiltActiveCy | Texture filtering cycles. This counter increments for every texture filtering issue cycle. Some instructions take more than one cycle due to multi-cycle data access and filtering operations: * 2D bilinear filtering takes two cycles per quad. * 2D trilinear filtering takes four cycles per quad. * 3D bilinear filtering takes four cycles per quad. * 3D trilinear filtering takes eight cycles per quad. Hardware name: TEX_FILT_NUM_OPERATIONS. | cycles |
| hwcpipe_gpu_mali_l2cache_rd_lookup | GpuCounter::CacheReadLookups | MaliL2CacheRdLookup | Read lookup requests. This counter increments for every L2 cache read lookup made. Hardware name: L2_READ_LOOKUP. | lookups |
| hwcpipe_gpu_mali_l2cache_wr_lookup | GpuCounter::CacheWriteLookups | MaliL2CacheWrLookup | Write lookup requests. This counter increments for every L2 cache write lookup made. Hardware name: L2_WRITE_LOOKUP. | lookups |
| hwcpipe_gpu_mali_ext_bus_rd | GpuCounter::ExternalMemoryReadAccesses | MaliExtBusRd | Output external read transactions. This counter increments for every external read transaction made on the memory bus. These transactions will typically result in an external DRAM access, but some designs include a system cache which can provide some buffering. The longest memory transaction possible is 64 bytes in length, but shorter transactions can be generated in some circumstances. Hardware name: L2_EXT_READ. | accesses |
| hwcpipe_gpu_mali_ext_bus_wr | GpuCounter::ExternalMemoryWriteAccesses | MaliExtBusWr | Output external write transactions. This counter increments for every external write transaction made on the memory bus. These transactions will typically result in an external DRAM access, but some chips include a system cache which can provide some buffering. The longest memory transaction possible is 64 bytes in length, but shorter transactions can be generated in some circumstances. Hardware name: L2_EXT_WRITE. | accesses |
| hwcpipe_gpu_mali_ext_bus_rd_stall_cy | GpuCounter::ExternalMemoryReadStalls | MaliExtBusRdStallCy | Output external read stall transactions. This counter increments for every stall cycle on the AXI bus where the GPU has a valid read transaction to send, but is awaiting a ready signal from the bus. Hardware name: L2_EXT_AR_STALL. | stalls |
| hwcpipe_gpu_mali_ext_bus_wr_stall_cy | GpuCounter::ExternalMemoryWriteStalls | MaliExtBusWrStallCy | Output external write stall cycles. This counter increments for every stall cycle on the external bus where the GPU has a valid write transaction to send, but is awaiting a ready signal from the external bus. Hardware name: L2_EXT_W_STALL. | stalls |
| hwcpipe_gpu_mali_ext_bus_rd_by | GpuCounter::ExternalMemoryReadBytes | MaliExtBusRdBy | Output external read bytes. This expression defines the output read bandwidth for the GPU. | B |
| hwcpipe_gpu_mali_ext_bus_wr_by | GpuCounter::ExternalMemoryWriteBytes | MaliExtBusWrBy | Output external write bytes. This expression defines the output write bandwidth for the GPU. | B |

## Reporting issues

If you encounter a problem, please report it as an [issue on GitHub](https://github.com/ylz-at/arm-hwcpipe-exporter/issues).

## License

This package is licensed under [The MIT License](https://mit-license.org/).

