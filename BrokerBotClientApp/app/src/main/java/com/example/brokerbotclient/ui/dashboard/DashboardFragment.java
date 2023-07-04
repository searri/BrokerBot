package com.example.brokerbotclient.ui.dashboard;

import static android.content.Context.MODE_PRIVATE;
import static com.example.brokerbotclient.MainActivity.DEFAULT_IP;
import static com.example.brokerbotclient.MainActivity.GAME_PREFERENCES;
import static com.example.brokerbotclient.MainActivity.isFormattedLikeIPAddress;

import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.brokerbotclient.R;
import com.example.brokerbotclient.RequestQueueSingleton;
import com.example.brokerbotclient.databinding.FragmentDashboardBinding;

import java.lang.reflect.Field;

public class DashboardFragment extends Fragment {

    private FragmentDashboardBinding binding;

    public void pingBrokerBotCloud() {
        RequestQueue queue = RequestQueueSingleton.getInstance(getActivity().getApplicationContext()).getRequestQueue();
        SharedPreferences settings = getActivity().getSharedPreferences(GAME_PREFERENCES, MODE_PRIVATE);
        String cloudAddress = settings.getString("IP", DEFAULT_IP);
        String url = "http://" + cloudAddress + "/state";

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest
                (Request.Method.GET, url, null, response -> {
                    Log.d("BUTTONS", "Response is: " + response.toString());
                    getView().findViewById(R.id.connectiontest).setBackgroundColor(Color.rgb(8, 163, 86));
                }, error -> {
                    Log.d("BUTTONS", error.toString());
                    Log.d("BUTTONS", "That didn't work!");
                    getView().findViewById(R.id.connectiontest).setBackgroundColor(Color.rgb(209, 38, 19));
                });

        // Add the request to the RequestQueue
        queue.add(jsonObjectRequest);
    }

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        DashboardViewModel dashboardViewModel =
                new ViewModelProvider(this).get(DashboardViewModel.class);

        binding = FragmentDashboardBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final TextView textView = binding.textDashboard;
        dashboardViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);

        Button dashboardIPSetButton = root.findViewById(R.id.new_ip_setbutton);
        dashboardIPSetButton.setOnClickListener(v -> {
            String newIP = ((EditText)root.findViewById(R.id.new_ip_textfield)).getText().toString();
            if (isFormattedLikeIPAddress(newIP)) {
                SharedPreferences settings = getActivity().getSharedPreferences(GAME_PREFERENCES, MODE_PRIVATE);
                SharedPreferences.Editor prefEditor = settings.edit();
                prefEditor.putString("IP", newIP);
                prefEditor.commit();
                dashboardViewModel.updatemText();
            } else {
                Log.d("BUTTONS", "Not an IP!");
                Toast.makeText(getActivity(), "Not a valid IP", Toast.LENGTH_LONG).show();
            }
        });

        Button testConnection = root.findViewById(R.id.connectiontest);
        testConnection.setOnClickListener(v -> {
            pingBrokerBotCloud();
        });

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}