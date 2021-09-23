package at.ylz.hwcpipe_exporter;

import static io.vertx.core.impl.FileResolver.DISABLE_CP_RESOLVING_PROP_NAME;
import static io.vertx.core.impl.FileResolver.DISABLE_FILE_CACHING_PROP_NAME;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import at.ylz.hwcpipe_exporter.databinding.ActivityMainBinding;

import io.prometheus.client.vertx.MetricsHandler;
import io.vertx.core.Vertx;
import io.vertx.ext.web.Router;


public class MainActivity extends AppCompatActivity {

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // setup simpleclient_vertx
        System.setProperty(DISABLE_FILE_CACHING_PROP_NAME, "true");
        System.setProperty(DISABLE_CP_RESOLVING_PROP_NAME, "true");
        final Vertx vertx = Vertx.vertx();
        final Router router = Router.router(vertx);
        router.route("/metrics").handler(new MetricsHandler());
        vertx.createHttpServer().requestHandler(router::accept).listen(9998);
        new HWCPipeExports().register();

        // print status
        ActivityMainBinding binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        TextView tv = binding.sampleText;
        tv.setText("ARM HWCPipe Counter Exporter Started");
    }

}