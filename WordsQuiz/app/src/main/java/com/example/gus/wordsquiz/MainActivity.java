package com.example.gus.wordsquiz;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.View;
import android.widget.Button;

import java.util.ArrayList;

public class MainActivity extends Activity {
    static ArrayList<Word> easyWords;
    static ArrayList<Word> mediumWords;
    static ArrayList<Word> hardWords;
    Button startBt;
    Button aboutBt;
    static private long lastClickTime = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        hardWords = DataUtils.fillHardWithWords();
        mediumWords = DataUtils.fillMediumWithWords();
        easyWords = DataUtils.fillEasyWithWords();
        startBt = findViewById(R.id.bt_start);
        aboutBt = findViewById(R.id.bt_about);
        startBt.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {
                preventDoubleClick();
                DialogFragment chooseDifficulty = new DifficultyDialogFragment();
                chooseDifficulty.show(getFragmentManager(), "Difficulty");

            }
        });
        aboutBt.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {
                preventDoubleClick();
                Intent aboutIntent = new Intent(MainActivity.this, AboutActivity.class);
                startActivity(aboutIntent);
            }
        });
    }

    void preventDoubleClick() {
        if (SystemClock.elapsedRealtime() - lastClickTime < 1000) {
            return;
        }
        lastClickTime = SystemClock.elapsedRealtime();
    }

    public static class DifficultyDialogFragment extends DialogFragment {
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Choose Difficulty")
                    .setItems(R.array.difficulty_levels_array, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            String[] difficultyLevels = getResources().getStringArray(R.array.difficulty_levels_array);
                            Intent startGame = new Intent(getActivity(), GameActivity.class);
                            startGame.putExtra("difficulty", difficultyLevels[which]);
                            startActivity(startGame);
                        }
                    });
            return builder.create();
        }
    }
}
