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
        Button AcceptButton;
        ListView ModeList;
        SeekBar seekBarHue, seekBarSat, seekBarVal;
        View colorView;
        BluetoothSocket btSocket;
        String[] configCodes;
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);
            configCodes = Resources.GetStringArray(Resource.Array.ConfigCodes);
            ModeList = FindViewById<ListView>(Resource.Id.ModeListView);
            colorView = FindViewById<View>(Resource.Id.ColorExampleView);
            ArrayAdapter<String> lvAdapter = new ArrayAdapter<String>(this, Android.Resource.Layout.SimpleListItemSingleChoice, configCodes);
            seekBarHue = FindViewById<SeekBar>(Resource.Id.seekBar1);
            seekBarSat = FindViewById<SeekBar>(Resource.Id.seekBar2);
            seekBarVal = FindViewById<SeekBar>(Resource.Id.seekBar3);
            seekBarHue.Max = 360;
            seekBarSat.Max = 255;
            seekBarVal.Max = 255;

     

            seekBarHue.ProgressChanged += (object sender, SeekBar.ProgressChangedEventArgs e) =>
            {
                if (e.FromUser)
                {
                    ColorDrawable color = (ColorDrawable)colorView.Background;
                    uint colorCode = 0;
                    double Hue = e.Progress;

                    int r, g, b;
                    double h, s, v;
                    h = seekBarHue.Progress;
                    s = (float)seekBarSat.Progress / 255;
                    v = (float)seekBarVal.Progress / 255;
                    HsvToRgb(h, s, v, out r, out g, out b);
                    colorCode += (uint)r;
                    colorCode += (uint)g * 0x100;
                    colorCode += (uint)b * 0x10000;
                    colorCode += (uint)color.Color.A * 0x1000000;
                    String colorString = "#" + colorCode.ToString("X");
                    Color ccolor = Color.ParseColor(colorString);
                    colorView.SetBackgroundColor(ccolor);
                    TextView HueText = FindViewById<TextView>(Resource.Id.textView1);
                    HueText.Text = "Тон: " + e.Progress;
                    
                    

                }
            };

            seekBarSat.ProgressChanged += (object sender, SeekBar.ProgressChangedEventArgs e) =>
            {
                if (e.FromUser)
                {
                    ColorDrawable color = (ColorDrawable)colorView.Background;
                    uint colorCode = 0;
                    int r, g, b;
                    double h, s, v;
                    h = seekBarHue.Progress;
                    s = (float)seekBarSat.Progress / 255;
                    v = (float)seekBarVal.Progress / 255;
                    HsvToRgb(h, s, v, out r, out g, out b);
                    colorCode += (uint)r;
                    colorCode += (uint)g * 0x100;
                    colorCode += (uint)b * 0x10000;
                    colorCode += (uint)color.Color.A * 0x1000000;
                    String colorString = "#" + colorCode.ToString("X");
                    Color ccolor = Color.ParseColor(colorString);
                    colorView.SetBackgroundColor(ccolor);
                    TextView SatText = FindViewById<TextView>(Resource.Id.textView2);
                    SatText.Text = "Насыщенность: " + e.Progress;
                }
            };

            seekBarVal.ProgressChanged += (object sender, SeekBar.ProgressChangedEventArgs e) =>
            {
                if (e.FromUser)
                {
                    ColorDrawable color = (ColorDrawable)colorView.Background;
                    uint colorCode = 0;
                    int r, g, b;
                    double h, s, v;
                    h = seekBarHue.Progress;
                    s = (float)seekBarSat.Progress / 255;
                    v = (float)seekBarVal.Progress / 255;
                    HsvToRgb(h, s, v, out r, out g, out b);
                    colorCode += (uint)r;
                    colorCode += (uint)g * 0x100;
                    colorCode += (uint)b * 0x10000;
                    colorCode += (uint)color.Color.A * 0x1000000;
                    String colorString = "#" + colorCode.ToString("X");
                    Color ccolor = Color.ParseColor(colorString);
                    colorView.SetBackgroundColor(ccolor);
                  
                    TextView ValText = FindViewById<TextView>(Resource.Id.textView3);
                    ValText.Text = "Яркость: " + e.Progress;
                }
            };
            
            ModeList.Adapter = lvAdapter;
            ModeList.ChoiceMode = ChoiceMode.Single;
            ModeList.SetSelection(0);
            ModeList.SetItemChecked(0, true);
            AcceptButton = FindViewById<Button>(Resource.Id.button1);
            AcceptButton.Click += delegate { AcceptButtonClick(); };
            

            BluetoothAdapter btAdapter = BluetoothAdapter.DefaultAdapter;
            
            if (btAdapter == null)
                throw new Exception("No Bluetooth adapter found.");

            if (!btAdapter.IsEnabled)
                throw new Exception("Bluetooth adapter is not enabled.");

            BluetoothDevice device = (from bd in btAdapter.BondedDevices
                                      where bd.Name == "ColorMusic"
                                      select bd).FirstOrDefault();
            if (device == null)
                throw new Exception("Named device not found.");
            btSocket = device.CreateInsecureRfcommSocketToServiceRecord(UUID.FromString(btUUID));
            try
            {
                btSocket.Connect();
        
            }
            catch (IOException e)
            {
                btSocket.Close();
                throw new Exception("Can't Connect");

               

            }

        }

        void AcceptButtonClick()
        {
            
            byte[] buf;
            switch(ModeList.CheckedItemPosition)
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
        }

        public void HsvToRgb(double h, double S, double V, out int r, out int g, out int b)
        {
            double H = h;
            while (H < 0) { H += 360; };
            while (H >= 360) { H -= 360; };
            double R, G, B;
            if (V <= 0)
            { R = G = B = 0; }
            else if (S <= 0)
            {
                R = G = B = V;
            }
            else
            {
                double hf = H / 60.0;
                int i = (int)Math.Floor(hf);
                double f = hf - i;
                double pv = V * (1 - S);
                double qv = V * (1 - S * f);
                double tv = V * (1 - S * (1 - f));
                switch (i)
                {
                    

                    case 0:
                        R = V;
                        G = tv;
                        B = pv;
                        break;
                        

                    case 1:
                        R = qv;
                        G = V;
                        B = pv;
                        break;
                    case 2:
                        R = pv;
                        G = V;
                        B = tv;
                        break;
                        

                    case 3:
                        R = pv;
                        G = qv;
                        B = V;
                        break;
                    case 4:
                        R = tv;
                        G = pv;
                        B = V;
                        break;
                        

                    case 5:
                        R = V;
                        G = pv;
                        B = qv;
                        break;
                        
                    case 6:
                        R = V;
                        G = tv;
                        B = pv;
                        break;
                    case -1:
                        R = V;
                        G = pv;
                        B = qv;
                        break;

                    // The color is not defined, we should throw an error.

                    default:
                        //LFATAL("i Value error in Pixel conversion, Value is %d", i);
                        R = G = B = V; // Just pretend its black/white
                        break;
                }
            }
            r = Clamp((int)(R * 255.0));
            g = Clamp((int)(G * 255.0));
            b = Clamp((int)(B * 255.0));
        }
        public double hue(int h)
        {
            return (double)h / 255 * 360;
        }

        public double sat(int s)
        {
            return (double)s / 255;
        }

        public double val(int v)
        {
            return (double)v / 255;
        }

        /// <summary>
        /// Clamp a value to 0-255
        /// </summary>
        int Clamp(int i)
        {
            if (i < 0) return 0;
            if (i > 255) return 255;
            return i;
        }

        public static void RGBToHSV(double r, double g, double b, out double hue, out double saturation, out double value)
        {
            double max = Math.Max(r, Math.Max(g, b));
            double min = Math.Min(r, Math.Min(g, b));
            double delta = max - min;
            value = max;
            if(max!=0)
            {
                saturation = delta / max;
            }
            else
            {
                saturation = 0;
                hue = 0;
                return;
            }
            if(r==max)
            {
                hue = (g - b) / delta;
            }
            else if (g == max)
            {
                hue = 2 + (b - r) / delta;
            }
            else
            {
                hue = 4 + (r - g) / delta;
            }
            hue *= 60;
            if (hue < 0)
            {
                hue += 360;
            }
            saturation = (max == 0) ? 0 : 1 - (1 * min / max);
            value = max / 255d;
        }
        

    }

}