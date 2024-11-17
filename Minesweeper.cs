using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Resources;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Minesweeper
{
    enum Difficulty
    {
        Easy, 
        Normal,
        High
    }

    public partial class Minesweeper : Form
    {
        private static int numberOfMines;
        private static int numberOfButtons;
        private static Difficulty difficulty;
        private static int rows;
        private static int columns;
        // number of correctly placed flags (on top of mines)
        private static int flagsOnMine;
        private static int time;
        // controls timer start only after Start is pressed
        private static bool timerFlag;
        // buttons on the minefield
        private System.Windows.Forms.Button[,] arrayOfButtons;
        // states of buttons on the field
        private int[,] arrayOfStates;
        // arrayOfIndexes[0] = rows, arrayOfIndexes[1] = cols
        private int[] arrayOfIndexes;

        public Minesweeper()
        {
            // easy - 10, normal - 40, hard - 99
            numberOfMines = 40;
            // easy - 81(9*9), normal - 256(16*16), hard - 496 (16*31) 
            numberOfButtons = 256;
            rows = 16;
            columns = 16;
            // 1 - easy, 2 - normal, 3 - hard
            difficulty = Difficulty.Normal; 
            timerFlag = false; 
            InitializeComponent();

            createObjects();
            startGame();
            // show all mines locations - "-" means there's a mine 
            /*for (int r = 0; r < rows; r++)
				for (int c = 0; c < columns; c++)
					if(arrayOfStates[r, c] == -1)
						arrayOfButtons[r, c].Text = "-";*/
			
        }

        void createObjects()
        {
            arrayOfButtons = new System.Windows.Forms.Button[rows, columns];
            arrayOfStates = new int[rows, columns];

            for (int r = 0; r < rows; r++)
                for (int c = 0; c < columns; c++)
                {
                    arrayOfIndexes = new int[2];
                    arrayOfIndexes[0] = c; // row
                    arrayOfIndexes[1] = r; // column
                    arrayOfButtons[r, c] = new System.Windows.Forms.Button();
                    arrayOfButtons[r, c].Location = new Point(r * 18, c * 18);
                    arrayOfButtons[r, c].Name = "Button";
                    arrayOfButtons[r, c].Size = new Size(20, 20);
                    arrayOfButtons[r, c].UseVisualStyleBackColor = true;
                    arrayOfButtons[r, c].BackColor = Color.Gainsboro;
                    arrayOfButtons[r, c].Font = new Font("Microsoft Sans Serif", 8.25F, FontStyle.Bold, GraphicsUnit.Point);
                    arrayOfButtons[r, c].TabStop = true;
                    arrayOfButtons[r, c].Tag = arrayOfIndexes;
                    arrayOfButtons[r, c].Click += new EventHandler(buttonOnField_Click);
                    arrayOfButtons[r, c].MouseDown += new MouseEventHandler(buttonOnField_Mouse_Click);
                    fieldPanel.Controls.Add(arrayOfButtons[r, c]);

                    arrayOfStates[r, c] = 0;  // reset states (0 means no bombs nearby)
        }
    }

    void startGame()
        {
            // start time in seconds
            time = 0;
            timerFlag = false;
            // no flags yet
            flagsOnMine = 0;
            // set initial mines count in textbox
            if (numberOfMines > 99)
                richTextBoxMines.Text = String.Format("{0}", numberOfMines);
            else
                richTextBoxMines.Text = String.Format("0{0}", numberOfMines);

            buttonStart.BackgroundImage = new Bitmap(Properties.Resources.start);

            putMines(); // put mines on the field
            calculateNumbers(); // put numbers around mines
        }

        void putMines()
        {
            // number of allocated mines 
            int n = 0;
            // cells indexes on the field
            int r, c;
            Random rnd = new Random();
            do
            {
                r = rnd.Next(rows);
                c = rnd.Next(columns);
                if (arrayOfStates[r, c] != -1)
                {
                    arrayOfStates[r, c] = -1;
                    n++;
                }
            } while (n != numberOfMines);
        }

        void calculateNumbers()
        {
            for (int r = 0; r < rows; r++)
                for (int c = 0; c < columns; c++)
                {
                    // if there's a bomb, then place the numbers around it
                    if (arrayOfStates[r, c] == -1) 
                    {
                        //if (r != rows -1)
                        {
                            if (c > 0) // if we didn't reach minefield bounds
                                if (arrayOfStates[r, c - 1] != -1) arrayOfStates[r, c - 1]++; //go to left
                            if (c < rows - 1)
                                if (arrayOfStates[r, c + 1] != -1) arrayOfStates[r, c + 1]++; // go to right
                            if (c > 0 && r > 0)
                                if (arrayOfStates[r - 1, c - 1] != -1) arrayOfStates[r - 1, c - 1]++; // go to top left
                            if (r > 0)
                                if (arrayOfStates[r - 1, c] != -1) arrayOfStates[r - 1, c]++; // go to top
                            if (r > 0 && c < rows - 1)
                                if (arrayOfStates[r - 1, c + 1] != -1) arrayOfStates[r - 1, c + 1]++; // go to top right
                            if (c > 0 && r < columns - 1)
                                if (arrayOfStates[r + 1, c - 1] != -1) arrayOfStates[r + 1, c - 1]++; // go to bottom left
                            if (r < columns - 1)
                                if (arrayOfStates[r + 1, c] != -1) arrayOfStates[r + 1, c]++; // go to bottom
                            if (c < rows - 1 && r < columns - 1)
                                if (arrayOfStates[r + 1, c + 1] != -1) arrayOfStates[r + 1, c + 1]++; // go to bottom right
                        }
                    }
                }
        }

        void refreshField()
        {
            // set initial mines count in textbox
            if (numberOfMines > 99)
                richTextBoxMines.Text = String.Format("{0}", numberOfMines);
            else
                richTextBoxMines.Text = String.Format("0{0}", numberOfMines);

            richTextBoxTime.Text = "000";

            for (int r = 0; r < rows; r++)
                for (int c = 0; c < columns; c++)
                {
                    // if lost, need to init the click handlers
                    arrayOfButtons[r, c].Click += new EventHandler(buttonOnField_Click);
                    arrayOfButtons[r, c].Enabled = true;
                    arrayOfButtons[r, c].BackColor = Color.Gainsboro;
                    arrayOfButtons[r, c].Text = "";
                    arrayOfButtons[r, c].BackgroundImage = null;
                    arrayOfButtons[r, c].MouseDown += new MouseEventHandler(buttonOnField_Mouse_Click);
                    arrayOfStates[r, c] = 0; // reset states(0 means no bombs nearby)
                }
        }

        // pain the buttons raskraska knopki
        void paintButton(int r, int c)
        {
            arrayOfButtons[r, c].BackColor = Color.DarkBlue;
            switch (arrayOfStates[r, c])
            {
                case 1:
                    arrayOfButtons[r, c].ForeColor = Color.Aquamarine;
                    break;
                case 2:
                    arrayOfButtons[r, c].ForeColor = Color.Orange;
                    break;
                case 3:
                    arrayOfButtons[r, c].ForeColor = Color.DeepPink;
                    break;
                case 4:
                    arrayOfButtons[r, c].ForeColor = Color.OliveDrab;
                    break;
                case 5:
                    arrayOfButtons[r, c].ForeColor = Color.GreenYellow;
                    break;
                case 6:
                    arrayOfButtons[r, c].ForeColor = Color.FloralWhite;
                    break;
                case 7:
                    arrayOfButtons[r, c].ForeColor = Color.Lime;
                    break;
                case 8:
                    arrayOfButtons[r, c].ForeColor = Color.Red;
                    break;
            }
            arrayOfButtons[r, c].Text = Convert.ToString(arrayOfStates[r, c]);
            // disable clicks		
            arrayOfButtons[r, c].MouseDown -= new MouseEventHandler(buttonOnField_Mouse_Click);
            arrayOfButtons[r, c].Click -= new EventHandler(buttonOnField_Click);
        }

        // recursively open buttons with 0 value (no mine)
        // r, c - indexes       
        // always set to true. if it's 1st method call in stack then it will stay true; if has been called recursively then false
        void openFree(int r, int c, bool userClick)
        {
            if (arrayOfStates[r, c] != 0)
            {
                if (!userClick) return;
                if (arrayOfStates[r, c] == -1)
                { // if found the mine
                    for (int i = 0; i < rows; i++)
                        for (int j = 0; j < columns; j++)
                        { // open the mines on the field
                            if (Convert.ToInt32(arrayOfStates[i, j]) == -1)
                            { // if mine - load the image 
                                arrayOfButtons[i, j].BackgroundImage = Properties.Resources.mine;
                                arrayOfButtons[i, j].BackgroundImageLayout = ImageLayout.Zoom;
                            }
                            else
                            { // if put the mines not correctly 
                                if (Convert.ToInt32(arrayOfStates[i, j]) != -1 && arrayOfButtons[i, j].BackgroundImage != null)
                                {
                                    arrayOfButtons[i, j].BackgroundImage = Properties.Resources.notMine;
                                    arrayOfButtons[i, j].BackgroundImageLayout = ImageLayout.Zoom;
                                    arrayOfButtons[i, j].Text = "";
                                }
                            }
                            // disable clicks
                            arrayOfButtons[i, j].Click -= new EventHandler(buttonOnField_Click);
                            arrayOfButtons[i, j].MouseDown -= new MouseEventHandler(buttonOnField_Mouse_Click);

                        }
                    // lost
                    buttonStart.BackgroundImage = Properties.Resources.looser;
                    buttonStart.BackgroundImageLayout = ImageLayout.Zoom;
                    stopTimer();
                    return;
                }
                return;
            }

            // paint pressed button 
            arrayOfButtons[r, c].Text = "";
            arrayOfButtons[r, c].ForeColor = Color.DarkBlue;
            // disable clicks, if flag is not on the button
            if (arrayOfButtons[r, c].BackgroundImage == null)
            {
                arrayOfButtons[r, c].MouseDown -= new MouseEventHandler(buttonOnField_Mouse_Click);
                arrayOfButtons[r, c].Click -= new EventHandler(buttonOnField_Click);
            }

            arrayOfStates[r, c] = -2; // means we already checked this button

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    int posR = r + k;
                    int posC = c + l;
                    if (l != 2 && k != 2 && posR >= 0 && posR < rows && posC >= 0 && posC < columns
                        && arrayOfStates[posR, posC] != -1)
                    { // paint the numbers pokrasim cyferki
                        if (arrayOfButtons[posR, posC].BackgroundImage == null)
                        {
                            arrayOfButtons[posR, posC].BackColor = Color.DarkBlue;
                            if (arrayOfStates[posR, posC] != 0)
                            {
                                paintButton(posR, posC);
                                // disable the clicks zapret myshy na pole
                                arrayOfButtons[posR, posC].Click -= new EventHandler(buttonOnField_Click);
                                arrayOfButtons[posR, posC].MouseDown -= new MouseEventHandler(buttonOnField_Mouse_Click);
                            }
                            else
                            {
                                arrayOfButtons[posR, posC].Text = "";
                                arrayOfButtons[posR, posC].ForeColor = Color.DarkBlue;
                                arrayOfButtons[posR, posC].BackgroundImage = null;
                            }
                        }
                        // if we have more free buttons
                        if (arrayOfStates[posR, posC] == 0)
                        {
                            openFree(posR, posC, false);
                        }
                    }
                }
            }
        }

#pragma region Timer
        
        void runTimer()
        {
            timer.Interval = 1000;
            timer.Tick += new EventHandler(timer_Tick);
            timer.Start();
            timer.Enabled = true;
        }
        
        void stopTimer()
        {
            timer.Stop();
            timer.Enabled = false;
            timer.Tick -= new EventHandler(timer_Tick);
            timer.Interval = 1000;
        }
        private void timer_Tick(object sender, EventArgs e)
        {
            timer.Enabled = false;
            ++time;
            if (time < 10)
                richTextBoxTime.Text = String.Format("00{0}", time);
            else if (time < 100 && time >= 10)
                richTextBoxTime.Text = String.Format("0{0}", time);
            else
                richTextBoxTime.Text = String.Format("{0}", time);
            timer.Enabled = true;
        }
#pragma endregion

        private void buttonOnField_Click(object sender, EventArgs e)
        {
            if (!timerFlag) // start the timer
                runTimer();
            timerFlag = true;
            // change the image 
            buttonStart.BackgroundImage = Properties.Resources.start;
            buttonStart.BackgroundImageLayout = ImageLayout.Zoom;

            int[] temp = new int[2];
            System.Windows.Forms.Button currentButton = ((System.Windows.Forms.Button) sender);
            temp = (int[]) currentButton.Tag;
            if (arrayOfStates[temp[1], temp[0]] > 0)
            {
                paintButton(temp[1], temp[0]);
            }
            else
            {
                openFree(temp[1], temp[0], true);
            }

        }

        private void buttonOnField_Mouse_Click(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            { // handle right mouse click
                if (!timerFlag) // start the timer
                    runTimer();
                timerFlag = true;

                // change the image 
                buttonStart.BackgroundImage = Properties.Resources.start;
                buttonStart.BackgroundImageLayout = ImageLayout.Zoom;

                int[] temp = new int[2];
                System.Windows.Forms.Button currentButton = ((System.Windows.Forms.Button)sender);
                temp = (int[])currentButton.Tag;
                // check if there's already a flag
                if (arrayOfButtons[temp[1], temp[0]].BackgroundImage == null)
                {
                    arrayOfButtons[temp[1], temp[0]].BackgroundImage = Properties.Resources.flag;
                    arrayOfButtons[temp[1], temp[0]].BackgroundImageLayout = ImageLayout.Zoom;
                    arrayOfButtons[temp[1], temp[0]].Click -= new EventHandler(buttonOnField_Click);

                    // change mines count in text box
                    if (numberOfMines > 99)
                        richTextBoxMines.Text = String.Format("{0}", Convert.ToInt32(richTextBoxMines.Text) - 1);
                    else
                    {
                        int var = Convert.ToInt32(richTextBoxMines.Text);
                        if (var - 1 < -99)
                        {
                            richTextBoxMines.Text = "-00";
                        }
                        else
                        {
                            if (var - 1 < 0)
                                richTextBoxMines.Text = String.Format("{0}", Convert.ToInt32(richTextBoxMines.Text) - 1);
                            else
                                richTextBoxMines.Text = String.Format("0{0}", Convert.ToInt32(richTextBoxMines.Text) - 1);
                        }
                    }
                    if (arrayOfStates[temp[1], temp[0]] == -1)
                        flagsOnMine++; // if there's a flag on top of the mine

                    // if on all mines there's a flag and no flags in other places - end of the game
                    if (flagsOnMine == numberOfMines && Convert.ToInt32(richTextBoxMines.Text) == 0)
                    {
                        stopTimer(); // stop the timer
                        buttonStart.BackgroundImage = Properties.Resources.winner;
                        buttonStart.BackgroundImageLayout = ImageLayout.Zoom;
                        MessageBox.Show("You won!");
                    }
                }
                else
                {
                    arrayOfButtons[temp[1], temp[0]].BackgroundImage = null;
                    arrayOfButtons[temp[1], temp[0]].Click += new EventHandler(buttonOnField_Click);

                    // change mines count in text box
                    if (numberOfMines > 99)
                        richTextBoxMines.Text = String.Format("{0}", Convert.ToInt32(richTextBoxMines.Text) + 1);
                    else
                    {
                        int var = Convert.ToInt32(richTextBoxMines.Text);
                        if (var + 1 > numberOfMines)
                        {
                            richTextBoxMines.Text = Convert.ToString(numberOfMines);
                        }
                        else
                        {
                            if (var + 1 < -99)
                                richTextBoxMines.Text = "-00";
                            if (var + 1 > 0)
                                richTextBoxMines.Text = String.Format("0{0}", Convert.ToInt32(richTextBoxMines.Text) + 1);
                            else
                                richTextBoxMines.Text = String.Format("{0}", Convert.ToInt32(richTextBoxMines.Text) + 1);
                        }
                    }

                    if (arrayOfStates[temp[1], temp[0]] == -1)
                        flagsOnMine--; // if flag is on top of the mine

                    // if on all mines there's a flag and no flags in other places - end of the game
                    if (flagsOnMine == numberOfMines && Convert.ToInt32(richTextBoxMines.Text) == 0)
                    {
                        stopTimer(); // stop the timer
                        buttonStart.BackgroundImage = Properties.Resources.winner;
                        buttonStart.BackgroundImageLayout = ImageLayout.Zoom;
                        MessageBox.Show("You won!");
                    }
                }
            }
        }

        private void newGameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            stopTimer();
            refreshField(); // reset buttons on the field				
            startGame();

        }

        private void easyToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void normalToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void hardToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            stopTimer();
            refreshField(); // reset buttons on the field				
            startGame();
        }
    }
}
