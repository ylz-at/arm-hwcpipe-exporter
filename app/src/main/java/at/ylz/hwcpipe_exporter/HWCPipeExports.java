package at.ylz.hwcpipe_exporter;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import io.prometheus.client.Collector;
import io.prometheus.client.CounterMetricFamily;

/**
 * Exports the GPU and CPU of ARM hardware components via Prometheus client.
 * <p>
 * This includes stats like MaliFragActiveCy and MaliExtBusWrBt.
 * <p>
 * Example usage:
 * <pre>
 * {@code
 *   new HWCPipeExports().register();
 * }
 * </pre>
 */
public class HWCPipeExports extends Collector {
    // Used to load the 'hwcpipe_exporter' library on application startup.
    static {
        System.loadLibrary("hwcpipe_exporter");
    }

    /**
     * native methods
     */
    private native void start();

    private native Map<String, Double> sample();

    private native void stop();

    public HWCPipeExports() {
        start();
    }

    @Override
    public List<MetricFamilySamples> collect() {
        List<MetricFamilySamples> mfs = new ArrayList<>();
        Map<String, Double> sampleResult = sample();
        sampleResult.forEach((counter, value) -> {
            String[] attrs = counter.split("#");
            assert attrs.length == 3;
            mfs.add(new CounterMetricFamily("hwcpipe_" + attrs[0], "ARM HWCPipe Counter. " + attrs[1] + ". Unit " + attrs[2] + ".", value));
        });
        return mfs;
    }
}
