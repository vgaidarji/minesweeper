namespace Minesweeper
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        private void InitializeComponent()
        {
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.gameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newGameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.easyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.normalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.hardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.startPanel = new System.Windows.Forms.Panel();
            this.buttonStart = new System.Windows.Forms.Button();
            this.richTextBoxTime = new System.Windows.Forms.RichTextBox();
            this.richTextBoxMines = new System.Windows.Forms.RichTextBox();
            this.fieldPanel = new System.Windows.Forms.Panel();
            this.menuStrip.SuspendLayout();
            this.startPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gameToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(311, 24);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip";
            // 
            // gameToolStripMenuItem
            // 
            this.gameToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newGameToolStripMenuItem,
            this.toolStripSeparator1,
            this.easyToolStripMenuItem,
            this.normalToolStripMenuItem,
            this.hardToolStripMenuItem,
            this.toolStripSeparator2,
            this.exitToolStripMenuItem});
            this.gameToolStripMenuItem.Name = "gameToolStripMenuItem";
            this.gameToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.gameToolStripMenuItem.Text = "Game";
            // 
            // newGameToolStripMenuItem
            // 
            this.newGameToolStripMenuItem.Name = "newGameToolStripMenuItem";
            this.newGameToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.newGameToolStripMenuItem.Text = "New Game";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(129, 6);
            // 
            // easyToolStripMenuItem
            // 
            this.easyToolStripMenuItem.Name = "easyToolStripMenuItem";
            this.easyToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.easyToolStripMenuItem.Text = "Easy";
            // 
            // normalToolStripMenuItem
            // 
            this.normalToolStripMenuItem.Name = "normalToolStripMenuItem";
            this.normalToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.normalToolStripMenuItem.Text = "Normal";
            // 
            // hardToolStripMenuItem
            // 
            this.hardToolStripMenuItem.Name = "hardToolStripMenuItem";
            this.hardToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.hardToolStripMenuItem.Text = "Hard";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(129, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // startPanel
            // 
            this.startPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.startPanel.Controls.Add(this.buttonStart);
            this.startPanel.Controls.Add(this.richTextBoxTime);
            this.startPanel.Controls.Add(this.richTextBoxMines);
            this.startPanel.Location = new System.Drawing.Point(9, 27);
            this.startPanel.Name = "startPanel";
            this.startPanel.Size = new System.Drawing.Size(294, 46);
            this.startPanel.TabIndex = 2;
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(128, 2);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(41, 38);
            this.buttonStart.TabIndex = 0;
            this.buttonStart.UseVisualStyleBackColor = true;
            // 
            // richTextBoxTime
            // 
            this.richTextBoxTime.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.richTextBoxTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 17.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxTime.Location = new System.Drawing.Point(240, 4);
            this.richTextBoxTime.MaxLength = 3;
            this.richTextBoxTime.Name = "richTextBoxTime";
            this.richTextBoxTime.Size = new System.Drawing.Size(47, 36);
            this.richTextBoxTime.TabIndex = 2;
            this.richTextBoxTime.Text = "000";
            // 
            // richTextBoxMines
            // 
            this.richTextBoxMines.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.richTextBoxMines.Font = new System.Drawing.Font("Microsoft Sans Serif", 17.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxMines.Location = new System.Drawing.Point(3, 3);
            this.richTextBoxMines.MaxLength = 3;
            this.richTextBoxMines.Name = "richTextBoxMines";
            this.richTextBoxMines.Size = new System.Drawing.Size(47, 36);
            this.richTextBoxMines.TabIndex = 1;
            this.richTextBoxMines.Text = "000";
            // 
            // fieldPanel
            // 
            this.fieldPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.fieldPanel.Location = new System.Drawing.Point(9, 79);
            this.fieldPanel.Name = "fieldPanel";
            this.fieldPanel.Size = new System.Drawing.Size(294, 294);
            this.fieldPanel.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlDark;
            this.ClientSize = new System.Drawing.Size(311, 376);
            this.Controls.Add(this.fieldPanel);
            this.Controls.Add(this.startPanel);
            this.Controls.Add(this.menuStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MainMenuStrip = this.menuStrip;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MineSweeper";
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.startPanel.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem gameToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newGameToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem easyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem normalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hardToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Panel startPanel;
        private System.Windows.Forms.RichTextBox richTextBoxTime;
        private System.Windows.Forms.RichTextBox richTextBoxMines;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Panel fieldPanel;
    }
}

