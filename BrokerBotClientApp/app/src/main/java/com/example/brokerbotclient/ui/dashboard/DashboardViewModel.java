package com.example.brokerbotclient.ui.dashboard;

import static android.content.Context.MODE_PRIVATE;
import static com.example.brokerbotclient.MainActivity.DEFAULT_IP;
import static com.example.brokerbotclient.MainActivity.GAME_PREFERENCES;

import android.app.Application;
import android.content.SharedPreferences;

import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class DashboardViewModel extends AndroidViewModel {

    private final MutableLiveData<String> mText;

    public DashboardViewModel(Application app) {
        super(app);
        SharedPreferences settings = getApplication().getSharedPreferences(GAME_PREFERENCES, MODE_PRIVATE);
        String cloudAddress = settings.getString("IP", DEFAULT_IP);

        mText = new MutableLiveData<>();
        mText.setValue("Current IP:\n" + cloudAddress);
    }

    public LiveData<String> getText() {
        return mText;
    }

    public void updatemText() {
        SharedPreferences settings = getApplication().getSharedPreferences(GAME_PREFERENCES, MODE_PRIVATE);
        String cloudAddress = settings.getString("IP", DEFAULT_IP);
        mText.setValue("Current IP:\n" + cloudAddress);
    }
}