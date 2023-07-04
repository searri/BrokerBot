package com.example.brokerbotclient.ui.home;

import static android.content.Context.MODE_PRIVATE;
import static com.example.brokerbotclient.MainActivity.DEFAULT_IP;
import static com.example.brokerbotclient.MainActivity.GAME_PREFERENCES;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
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
import com.example.brokerbotclient.databinding.FragmentHomeBinding;

import org.json.JSONException;
import org.json.JSONObject;


public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;

    public void startGame(JSONObject startGameData) {
        RequestQueue queue = RequestQueueSingleton.getInstance(getActivity().getApplicationContext()).getRequestQueue();
        SharedPreferences settings = getActivity().getSharedPreferences(GAME_PREFERENCES, MODE_PRIVATE);
        String cloudAddress = settings.getString("IP", DEFAULT_IP);
        String url = "http://" + cloudAddress + "/start";

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest
                (Request.Method.POST, url, startGameData, response -> {
                    Log.d("BUTTONS", "Response is: " + response.toString());
                }, null);

        queue.add(jsonObjectRequest);
    }

    public void sendTurnRolls(JSONObject turnData) {
        RequestQueue queue = RequestQueueSingleton.getInstance(getActivity().getApplicationContext()).getRequestQueue();
        SharedPreferences settings = getActivity().getSharedPreferences(GAME_PREFERENCES, MODE_PRIVATE);
        String cloudAddress = settings.getString("IP", DEFAULT_IP);
        String url = "http://" + cloudAddress + "/stocks";

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest
                (Request.Method.POST, url, turnData, response -> {
                    Log.d("BUTTONS", "Response is: " + response.toString());
                }, error -> {
                    Log.d("BUTTONS", "Error: " + error.toString());
                    Toast.makeText(getActivity(), "Connection error", Toast.LENGTH_SHORT).show();
                });

        // Add the request to the RequestQueue
        queue.add(jsonObjectRequest);
    }

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        HomeViewModel homeViewModel =
                new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final TextView textView = binding.textHome;
        homeViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);

        Button startGame = root.findViewById(R.id.startgame);
        startGame.setOnClickListener(v -> {
            int gameYears = Integer.parseInt(((EditText) root.findViewById(R.id.enterYears)).getText().toString());
            if (gameYears < 1) {
                Toast.makeText(getActivity(), "Invalid Game Length", Toast.LENGTH_LONG).show();
            } else {
                try {
                    JSONObject startGamePayload = new JSONObject("{\"num_years\": " + gameYears + "}");
                    startGame(startGamePayload);
                } catch (JSONException e) {
                    Log.d("BUTTONS", "Error: " + e);
                }
            }
            homeViewModel.updatemText();
        });

        Button updatePrices = root.findViewById(R.id.sendbutton);
        updatePrices.setOnClickListener(v -> {
            String bullOrBear = ((Spinner) root.findViewById(R.id.marketTypeSpinner)).getSelectedItem().toString();
            boolean isBull = bullOrBear.equals("Bull");
            int diceRoll = Integer.parseInt(((Spinner) root.findViewById(R.id.marketspinner)).getSelectedItem().toString());
            int eventCard = Integer.parseInt(((EditText) root.findViewById(R.id.eventCard)).getText().toString());
            if (eventCard < 1 || eventCard > 36) {
                Toast.makeText(getActivity(), "Invalid Event Card number", Toast.LENGTH_LONG).show();
            } else {
                try {
                    JSONObject gameUpdatePayload = new JSONObject("{\"is_bull\": " + isBull + ", \"roll\":" + diceRoll + ", \"event\":" + eventCard + "}");
                    sendTurnRolls(gameUpdatePayload);
                } catch (JSONException e) {
                    Log.d("BUTTONS", "Error: " + e);
                }
            }
            homeViewModel.updatemText();
        });

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}