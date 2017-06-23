using Android.App;
using Android.Widget;
using Android.OS;
using Android.Bluetooth;
using Java.IO;
using Java.Util;
using System;
using System.Linq;
using Java.Net;
using Android.Content;
using System.Threading;
using System.Threading.Tasks;
using System.Net.Http;
using System.Text;
using Android.Views;
using Android.Graphics.Drawables;
using Android.Graphics;

namespace ColorMusic_Companion
{
    [Activity(Label = "ColorMusic_Companion", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        private const string btUUID = "00001101-0000-1000-8000-00805f9b34fb";
        Button button1;
        EditText edField;
        ListView lv;
        SeekBar seekBarRed, seekBarGreen, seekBarBlue;
        View colorView;
        BluetoothSocket btSocket;
        String[] configCodes;
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);
            edField = FindViewById<EditText>(Resource.Id.editText1);
            configCodes = Resources.GetStringArray(Resource.Array.ConfigCodes);
            lv = FindViewById<ListView>(Resource.Id.listView1);
            colorView = FindViewById<View>(Resource.Id.view1);
            ArrayAdapter<String> lvAdapter = new ArrayAdapter<String>(this, Android.Resource.Layout.SimpleListItemSingleChoice, configCodes);
            seekBarRed = FindViewById<SeekBar>(Resource.Id.seekBar1);
            seekBarGreen = FindViewById<SeekBar>(Resource.Id.seekBar2);
            seekBarBlue = FindViewById<SeekBar>(Resource.Id.seekBar3);
            seekBarRed.Max = 255;
            seekBarGreen.Max = 255;
            seekBarBlue.Max = 255;

            seekBarRed.ProgressChanged += (object sender, SeekBar.ProgressChangedEventArgs e) =>
            {
                if (e.FromUser)
                {
                    edField.Text = string.Format("The value of the SeekBar is {0}", e.Progress);
                    ColorDrawable color = (ColorDrawable)colorView.Background;
                    uint colorCode = 0;
                    colorCode += (uint)color.Color.B;
                    colorCode += (uint)color.Color.G * 0x100;
                    colorCode += (uint)e.Progress * 0x10000;
                    colorCode += (uint)color.Color.A * 0x1000000;
                    String colorString = "#" + colorCode.ToString("X");
                    Color ccolor = Color.ParseColor(colorString);
                    colorView.SetBackgroundColor(ccolor);
                    //edField.Text += colorString;

                }
            };

            seekBarGreen.ProgressChanged += (object sender, SeekBar.ProgressChangedEventArgs e) =>
            {
                if (e.FromUser)
                {
                    edField.Text = string.Format("The value of the SeekBar is {0}", e.Progress);
                    ColorDrawable color = (ColorDrawable)colorView.Background;
                    uint colorCode = 0;
                    colorCode += (uint)color.Color.B;
                    colorCode += (uint)e.Progress * 0x100;
                    colorCode += (uint)color.Color.R * 0x10000;
                    colorCode += (uint)color.Color.A * 0x1000000;
                    String colorString = "#" + colorCode.ToString("X");
                    Color ccolor = Color.ParseColor(colorString);
                    colorView.SetBackgroundColor(ccolor);
                    // edField.Text += colorString;

                }
            };

            seekBarBlue.ProgressChanged += (object sender, SeekBar.ProgressChangedEventArgs e) =>
            {
                if (e.FromUser)
                {
                    edField.Text = string.Format("The value of the SeekBar is {0}", e.Progress);
                    ColorDrawable color = (ColorDrawable)colorView.Background;
                    uint colorCode = 0;
                    colorCode = (uint)e.Progress; //color.Color.B;
                    colorCode += (uint)color.Color.G * 0x100;
                    colorCode += (uint)color.Color.R * 0x10000;
                    colorCode += (uint)color.Color.A * 0x1000000;
                    String colorString = "#" + colorCode.ToString("X");
                    Color ccolor = Color.ParseColor(colorString);
                    colorView.SetBackgroundColor(ccolor);
                    //edField.Text += colorString;

                }
            };
            //lv.SetAdapter(lvAdapter);
            lv.Adapter = lvAdapter;
            lv.ChoiceMode = ChoiceMode.Single;
            lv.SetSelection(0);
            lv.SetItemChecked(0, true);
            edField.Text += lv.SelectedItem.ToString() + " " + lv.SelectedItemPosition;

            button1 = FindViewById<Button>(Resource.Id.button1);
            BluetoothAdapter adapter = BluetoothAdapter.DefaultAdapter;
            button1.Click += delegate { button1OnClick(); };
            if (adapter == null)
                throw new Exception("No Bluetooth adapter found.");

            if (!adapter.IsEnabled)
                throw new Exception("Bluetooth adapter is not enabled.");

            BluetoothDevice device = (from bd in adapter.BondedDevices
                                          //where bd.Address == "00:21:13:00:BB:B9"
                                      where bd.Name == "HC-06\n"
                                      select bd).FirstOrDefault();
            if (device == null)
                throw new Exception("Named device not found.");
            btSocket = device.CreateInsecureRfcommSocketToServiceRecord(UUID.FromString(btUUID));
            try
            {
                btSocket.Connect();
                byte[] buf = Encoding.ASCII.GetBytes("config 85");
                btSocket.OutputStream.Write(buf, 0, buf.Length);
            }
            catch (IOException e)
            {
                btSocket.Close();
                throw new Exception("Can't Connect");

                /*foreach(string bd in adapter.BondedDevices)
                {
                    text1.Text += bd;
                    text1.Text += "\r\n";
                }*/

            }

        }

        void button1OnClick()
        {
            EditText ed1 = FindViewById<EditText>(Resource.Id.editText1);
            //ed1.Text += "Hi, WTF?";
            byte[] buf;// = Encoding.ASCII.GetBytes("config 0");
            switch(lv.CheckedItemPosition)
            {
                case 0:
                    buf = Encoding.ASCII.GetBytes("config 0");
                    break;
                case 1:
                    buf = Encoding.ASCII.GetBytes("config 85");
                    break;
                case 2:
                    buf = Encoding.ASCII.GetBytes("config 149");
                    break;
                case 3:
                    buf = Encoding.ASCII.GetBytes("config 22");
                    break;
                case 4:
                    buf = Encoding.ASCII.GetBytes("config 26");
                    break;
                default:
                    buf = Encoding.ASCII.GetBytes("config 85");
                    break;
            }
            btSocket.OutputStream.Write(buf,0,buf.Length);
            ed1.Text = lv.CheckedItemPosition.ToString();
        }

    }

}