package com.example.selectdeviceactivity

import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothSocket
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.widget.ProgressBar
import android.widget.TextView
import android.widget.ToggleButton
import com.example.selectdeviceactivity.QualityActivity.Companion.currentTime_surf
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import java.util.Date
import java.util.Locale
import java.util.UUID

class SurfActivity : AppCompatActivity() {

    companion object {

        fun updateAccelValue(value: String) {

            val cleanedValue = value?.replace("R", "")
            mpuValueTextView.text = cleanedValue
            Log.d("DAMN", cleanedValue!!)

        }

        private lateinit var mpuValueTextView: TextView


        val EXTRA_ADDRESS: String = "Device_address"
        private lateinit var userId: String
        private lateinit var m_address: String
        private val m_myUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
        private var m_bluetoothSocket: BluetoothSocket? = null
        private lateinit var m_progress: ProgressDialog
        private lateinit var m_bluetoothAdapter: BluetoothAdapter
        private var m_isConnected: Boolean = false
        private var sentCommand: String = ""
        private lateinit var database: DatabaseReference
        private var commandHandler: Handler? = null
        private var commandRunnable: Runnable? = null

        private lateinit var toggleButton_mode: ToggleButton


    }

    private var currentIndex: Int = 0
    private lateinit var valueEventListener_surf: ValueEventListener
    val surf_parameters = listOf("Roll", "Pitch", "xxx", "xxx")

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_surf)

        QualityActivity.currentTime_surf = getCurrentTime()

        mpuValueTextView = findViewById<TextView>(R.id.velocity_value)
        //userId = QualityActivity.userId


        // Remove the child reference based on currentTime_surf
        //val mpuValueRef = QualityActivity.database.child("users").child(QualityActivity.userId)
           // .child("AccelValues").child(currentTime_surf).child(surf_parameters[currentIndex])


        /*
        valueEventListener_surf = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                Log.d("DAMN", "onDataChange called")
                val value = dataSnapshot.getValue(String::class.java)
                if (value != null) {
                    Log.d("DAMN", value)
                }
                // Handle the data here based on the parameter name (surf_parameters[currentIndex])
                val cleanedValue = value?.replace("R", "")
                val roll = cleanedValue?.toFloatOrNull() ?: 0.0f
                cleanedValue?.let { updateTextView(it) }
            }
            override fun onCancelled(databaseError: DatabaseError) {
                Log.d("QualityActivity", "onCancelled: ${databaseError.message}")
            }
        }

        // Add the ValueEventListener to the database reference without using currentTime_surf
        mpuValueRef.addValueEventListener(valueEventListener_surf)

    }*/
    }

    private fun getCurrentTime(): String {
        val currentTimeMillis = System.currentTimeMillis()
        val sdf = java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())
        return sdf.format(Date(currentTimeMillis))
    }

    interface DataListener {
        fun onDataReceived(value: String)
    }
}
