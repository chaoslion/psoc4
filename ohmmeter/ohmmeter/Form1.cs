using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Text;
using System.IO.Ports;
using System.Globalization;
using System.IO;

namespace ohmmeter {
    public partial class frmMain : Form {
        enum app_state {
            NoPorts,
            Connected,
            Disconnected
        };

        private app_state state;
        private string uartBuffer;

        public frmMain() {
            InitializeComponent();
        }



        private void spUart_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            string data = this.spUart.ReadExisting();
            this.uartBuffer += data;            
            // get last reading
            int packet_start = this.uartBuffer.LastIndexOf("<");
            if( packet_start == -1 ) {
                // retry at next data
                return;
            }
            int packet_end = this.uartBuffer.LastIndexOf(">");
            if (packet_end == -1) {
                // retry at next data
                return;
            }
            string packet = this.uartBuffer.Substring(packet_start + 1, packet_end - packet_start - 1);            
            MethodInvoker inv = delegate {
                this.setDisplay(packet);
            };
            this.Invoke(inv);            
            this.uartBuffer = "";            
        }

        private string nice_ohm(double number) {
            string prefix = null;
            string[] prefixes = new string[]{
                "m", "", "k", "M", "G", "T"
            };
            
            int index = (int)Math.Truncate(
                Math.Log(number) / Math.Log(1000)
            );
            // add offset so that index 0 equals no prefix
            int prefix_index = index + 1;
            if (prefix_index >= 0 && prefix_index < prefixes.Length) {
                prefix = prefixes[prefix_index];
                number /= Math.Pow(1000, index);
            }

            number = Math.Round(number, 4);                       
            if( prefix != null ) {
                return Convert.ToString(number) + " " + prefix +  "\u03A9";
            }
            return Convert.ToString(number) + " \u03A9";
        }            

        private void setDisplay(string packet) {            
            string resistance = packet;            

            switch(packet) {
                case "OL":
                    resistance = "open";
                    break;
                case "CL":
                    resistance = "short";                                    
                    break;
                default:                    
                    // convert to num to float
                    double r = double.Parse(packet, CultureInfo.InvariantCulture.NumberFormat);
                    // apply nice num                
                    resistance = this.nice_ohm(r);                    
                    break;
            }
            this.tbMainDisplay.Text = resistance;            
        }

        private void update_state() {
            switch(this.state) {
                case app_state.Connected:
                    this.btnUartConnect.Text = "Disconnect";
                    this.tbMainDisplay.Enabled = true;                    
                    break;
                case app_state.Disconnected:
                    this.btnUartConnect.Text = "Connect";
                    this.tbMainDisplay.Clear();                    
                    this.tbMainDisplay.Enabled = false;                    
                    break;
                case app_state.NoPorts:
                    this.btnUartConnect.Text = "Connect";
                    this.btnUartConnect.Enabled = false;
                    this.cbUartList.Enabled = false;
                    this.tbMainDisplay.Enabled = false;                    
                    break;
            }            
        }

        private void frmMain_Load(object sender, EventArgs e) {
            // fill uart combo box
            foreach (string name in SerialPort.GetPortNames()) {
                this.cbUartList.Items.Add(name);
            }
            if( this.cbUartList.Items.Count > 0 ) {
                this.state = app_state.Disconnected;
                this.cbUartList.SelectedIndex = 0;
            } else {
                this.state = app_state.NoPorts;
            }
            // configure uart
            this.spUart.BaudRate = 115200;
            // configure gui
            update_state();            
        }

        private void btnUartConnect_Click(object sender, EventArgs e) {
            if( this.state == app_state.Disconnected ) {
                this.spUart.PortName = (string)this.cbUartList.SelectedItem;
                try {
                    this.spUart.Open();
                    this.spUart.DiscardInBuffer();
                    this.state = app_state.Connected;
                }
                catch(UnauthorizedAccessException ex) {
                    MessageBox.Show(ex.Message);
                }                                                                    
            } else {
                this.spUart.Close();
                this.state = app_state.Disconnected;
            }
            this.update_state();
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e) {
            if (this.state == app_state.Connected) {
                this.spUart.Close();
            }
        }
    }
}
