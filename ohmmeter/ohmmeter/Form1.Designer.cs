namespace ohmmeter
{
    partial class frmMain
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnUartConnect = new System.Windows.Forms.Button();
            this.cbUartList = new System.Windows.Forms.ComboBox();
            this.spUart = new System.IO.Ports.SerialPort(this.components);
            this.tbMainDisplay = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnUartConnect
            // 
            this.btnUartConnect.Location = new System.Drawing.Point(139, 12);
            this.btnUartConnect.Name = "btnUartConnect";
            this.btnUartConnect.Size = new System.Drawing.Size(121, 21);
            this.btnUartConnect.TabIndex = 0;
            this.btnUartConnect.Text = "button1";
            this.btnUartConnect.UseVisualStyleBackColor = true;
            this.btnUartConnect.Click += new System.EventHandler(this.btnUartConnect_Click);
            // 
            // cbUartList
            // 
            this.cbUartList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbUartList.FormattingEnabled = true;
            this.cbUartList.Location = new System.Drawing.Point(12, 12);
            this.cbUartList.Name = "cbUartList";
            this.cbUartList.Size = new System.Drawing.Size(121, 21);
            this.cbUartList.TabIndex = 1;
            // 
            // spUart
            // 
            this.spUart.BaudRate = 115200;
            this.spUart.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.spUart_DataReceived);
            // 
            // tbMainDisplay
            // 
            this.tbMainDisplay.BackColor = System.Drawing.Color.Black;
            this.tbMainDisplay.Font = new System.Drawing.Font("Digital-7", 42F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbMainDisplay.ForeColor = System.Drawing.Color.Red;
            this.tbMainDisplay.Location = new System.Drawing.Point(12, 39);
            this.tbMainDisplay.Name = "tbMainDisplay";
            this.tbMainDisplay.ReadOnly = true;
            this.tbMainDisplay.Size = new System.Drawing.Size(360, 63);
            this.tbMainDisplay.TabIndex = 2;
            this.tbMainDisplay.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(384, 114);
            this.Controls.Add(this.tbMainDisplay);
            this.Controls.Add(this.cbUartList);
            this.Controls.Add(this.btnUartConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.Text = "Ohmmeter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnUartConnect;
        private System.Windows.Forms.ComboBox cbUartList;
        private System.IO.Ports.SerialPort spUart;
        private System.Windows.Forms.TextBox tbMainDisplay;
    }
}

