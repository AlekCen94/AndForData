package com.digfor.andfordata;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.ContentResolver;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.CallLog;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    private  final int STORAGE_PERMISSION_CODE = 2;
    private static final String FILE_NAME = "Sms.txt";
    private static final String FILE_NAME_CALL = "call.txt";

    private Button m_ReadSms;
    private Button m_Close;
    private Button m_Calls;
    private TextView m_Chesk;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
    }
    protected String extractSms()
    {

        StringBuffer m_Buffer = new StringBuffer();

        Uri inboxURI = Uri.parse("content://sms/inbox");

        String[] reqCols = new String[] { "_id", "address", "body" };


        ContentResolver cr = getContentResolver();

        Cursor c = cr.query(inboxURI, reqCols, null, null, null);
        if(c.getCount()==0)
        {
            Toast.makeText(this,"Nema poruka",Toast.LENGTH_SHORT);
            return "Nema poruka";
        }
        else
        {
            while (c.moveToNext())
            {
                m_Buffer.append("Id: "+c.getString(0)+"&&Number: "+ c.getString(1)+"&&Message: "+c.getString(2)+"^\n");
            }
            return m_Buffer.toString();
        }
    }

    protected String extractCalls() {

        StringBuffer m_Buffer = new StringBuffer();

        Uri inboxURI = Uri.parse("content://call_log/calls");



        ContentResolver cr = getContentResolver();


        Cursor c = cr.query(inboxURI, null, null, null, null);

        if(c.getCount()==0)
        {
            Toast.makeText(this,"Nema poziva",Toast.LENGTH_SHORT);
            return "Nema poziva";
        }
        else
        {
            while (c.moveToNext())
            {
                m_Buffer.append("Number: "+c.getString(c.getColumnIndex(CallLog.Calls.NUMBER))+ "&&Name: "+ c.getString(c.getColumnIndex(CallLog.Calls.CACHED_NAME))+ "&&Duration:"+c.getString(c.getColumnIndex(CallLog.Calls.DURATION))+"&&Type: "+c.getString(c.getColumnIndex(CallLog.Calls.TYPE))+"^\n");
            }
            return m_Buffer.toString();
        }
    }



    private void requestStoragePermission() {
        if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE)||ActivityCompat.shouldShowRequestPermissionRationale(this,
                Manifest.permission.READ_CALL_LOG)||ActivityCompat.shouldShowRequestPermissionRationale(this,
                Manifest.permission.READ_SMS)) {
            new AlertDialog.Builder(this)
                    .setTitle("Permission needed")
                    .setMessage("This permission is needed because of this and that")
                    .setPositiveButton("ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            ActivityCompat.requestPermissions(MainActivity.this,
                                    new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.READ_CALL_LOG,Manifest.permission.READ_SMS}, STORAGE_PERMISSION_CODE);
                        }
                    })
                    .setNegativeButton("cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.dismiss();
                        }
                    })
                    .create().show();
        } else {
            ActivityCompat.requestPermissions(this,
                    new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.READ_SMS,Manifest.permission.READ_CALL_LOG}, STORAGE_PERMISSION_CODE);
        }
    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {

         if (requestCode == STORAGE_PERMISSION_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                //Toast.makeText(this, extractSms(), Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this, "Permission DENIED", Toast.LENGTH_SHORT).show();
            }
        }

    }

    @Override
    protected void onStart() {
        super.onStart();



       if (ContextCompat.checkSelfPermission(MainActivity.this,
                Manifest.permission.READ_SMS) == PackageManager.PERMISSION_GRANTED&&ContextCompat.checkSelfPermission(MainActivity.this,
               Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED&&ContextCompat.checkSelfPermission(MainActivity.this,
               Manifest.permission.READ_CALL_LOG) == PackageManager.PERMISSION_GRANTED) {
            Toast.makeText(MainActivity.this, "You have already granted this permission!",
                    Toast.LENGTH_SHORT).show();
        } else {
            requestStoragePermission();

        }



    }

private void init()
{
    m_ReadSms = (Button) findViewById(R.id.m_Sms_Button);
    m_Close = (Button) findViewById(R.id.m_Close_Button);
    m_Chesk = (TextView) findViewById(R.id.m_Check);
    m_Calls = (Button) findViewById(R.id.m_Calls_Button);

    m_ReadSms.setOnClickListener(new View.OnClickListener() {
    public void onClick(View v) {
        if (ContextCompat.checkSelfPermission(MainActivity.this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED&&ContextCompat.checkSelfPermission(MainActivity.this,
                Manifest.permission.READ_SMS) == PackageManager.PERMISSION_GRANTED) {
                    saveSms();
        } else {
            requestStoragePermission();
        }
    }
});
    m_Close.setOnClickListener(new View.OnClickListener() {
        public void onClick(View v) {
            finishAffinity();
            System.exit(0);
        }
    });

    m_Calls.setOnClickListener(new View.OnClickListener() {
        public void onClick(View v) {
            if (ContextCompat.checkSelfPermission(MainActivity.this,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED&&ContextCompat.checkSelfPermission(MainActivity.this,
                    Manifest.permission.READ_CALL_LOG) == PackageManager.PERMISSION_GRANTED) {
                saveCalls();
            } else {
                requestStoragePermission();
            }
        }
    });


    }

    public void saveSms() {

        File dir = new File(Environment.getExternalStorageDirectory()+"/"+"AndForData");
        if(!dir.exists()){
            dir.mkdir();
        }

        try {
            File gpxfile = new File(dir, FILE_NAME);
            FileWriter writer = new FileWriter(gpxfile);
            writer.append(extractSms());
            writer.flush();
            writer.close();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public void saveCalls() {

        File dir = new File(Environment.getExternalStorageDirectory()+"/"+"AndForData");
        if(!dir.exists()){
            dir.mkdir();
        }

        try {
            File gpxfile = new File(dir, FILE_NAME_CALL);
            FileWriter writer = new FileWriter(gpxfile);
            writer.append(extractCalls());
            writer.flush();
            writer.close();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    }



