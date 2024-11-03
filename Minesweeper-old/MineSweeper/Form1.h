#pragma once


namespace MineSweeper {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Resources;
	using namespace System::Reflection;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			this->numberOfMines = 40; // easy - 10, normal - 40, hard - 99
			this->numberOfButtons = 256; // easy - 81(9*9), normal - 256(16*16), hard - 496 (16*31) 
			this->rows = 16;	// 16 obiektov po gorizontali
			this->colums = 16; // 16 obiektov po vertikali
			this->difficulty = 2; // 1 - easy, 2 - normal, 3 - hard
			res = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			timerFlag = false; // timer eshe ne zapushen
			InitializeComponent();

			this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^ >(res->GetObject(L"start")));

			createObjects(); // sozdadim knopocki na forme
			startGame(); // nacalo igry
			//esli bomba, to -  "-" 
			/*for (int r = 0; r < rows; r++)
				for (int c = 0; c < colums; c++)
					if(this->arrayOfStates[r, c] == -1)
						this->arrayOfButtons[r, c]->Text = "-";
			*/
		}

	protected:

		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
#pragma region Fields
	private: System::Windows::Forms::MenuStrip^  menuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  gameToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  championsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  abToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  newGameToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  easyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  normalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  hardToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::Panel^  startPanel;

	private: System::Windows::Forms::Button^  buttonStart;
	private: static int numberOfMines; //kolicestvo min na pole
	private: static int numberOfButtons; //kolicestvo knopok na pole vsego
	private: static int difficulty; //slojnosti
	private: static int rows; //kolicestvo knopok v stroke
	private: static int colums; //kolicestvo knopok v stolbce
	private: static int flagsOnMine; // kolicestvo ustanovlennyh pravilno flajkov (sverhu min)
	private: static int time; // vremia
	private: static bool timerFlag; // ctoby timer zapuskalsia toliko pri pervom najatii na pole

	private: array<System::Windows::Forms::Button^,2> ^ arrayOfButtons; 
	private: System::Windows::Forms::Panel^  fieldPanel;
	private: array<int, 2>^  arrayOfStates; //massiv sostoyaniy knopocek
	private: array<int>^ arrayOfIndexes; // arrayOfIndexes[0] = rows, arrayOfIndexes[1] = cols
	private: System::ComponentModel::IContainer^  components;

	private: System::Windows::Forms::RichTextBox^  richTextBoxMines;
	private: System::Windows::Forms::RichTextBox^  richTextBoxTime;
	private: System::Windows::Forms::Timer^  timer;


	private: System::ComponentModel::ComponentResourceManager^  res; 
#pragma endregion
	protected: 
		void startGame()
		{
			time = 0; // nacalinoe vremia v sekundah
			timerFlag = false;
			this->flagsOnMine = 0; // iznacalino flagi ne ustanovleny
			//ustanovim kolicestvo min v richTextBox-e	
			if(this->numberOfMines > 99)
				this->richTextBoxMines->Text = Convert::ToString(this->numberOfMines);
			else 
				this->richTextBoxMines->Text = "0" + Convert::ToString(this->numberOfMines);

			this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"start")));
			this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;
			putMines(); // rasstavim miny na pole
			calculateNumbers(); // rasstavim ciferki vokrug bomb
		}
		void createObjects()
		{
			// sozdadim knopki na forme
			this->arrayOfButtons = gcnew array<System::Windows::Forms::Button ^,2>(this->rows, this->colums);
			this->arrayOfStates = gcnew array<int, 2>(this->rows, this->colums); //massiv sostoyaniy

			for(int r = 0; r < rows; r++)
				for(int c = 0; c < colums; c++)
				{
					this->arrayOfIndexes = gcnew array<int>(2);
					arrayOfIndexes[0] = c; //stroka
					arrayOfIndexes[1] = r; //stolbec
					this->arrayOfButtons[r, c] = (gcnew System::Windows::Forms::Button());
					this->arrayOfButtons[r, c]->Location = System::Drawing::Point(r * 18, c * 18);
					this->arrayOfButtons[r, c]->Name = L"Button";
					this->arrayOfButtons[r, c]->Size = System::Drawing::Size(20, 20);
					this->arrayOfButtons[r, c]->UseVisualStyleBackColor = true;
					this->arrayOfButtons[r, c]->BackColor = System::Drawing::Color::Gainsboro;
					this->arrayOfButtons[r, c]->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
						static_cast<System::Byte>(204)));
					this->arrayOfButtons[r, c]->TabStop = true;
					this->arrayOfButtons[r, c]->Tag = arrayOfIndexes;
					this->arrayOfButtons[r, c]->Click += gcnew System::EventHandler(this, &Form1::buttonOnField_Click);
					this->arrayOfButtons[r, c]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::buttonOnField_Mouse_Click);
					this->fieldPanel->Controls->Add(this->arrayOfButtons[r,c]);

					this->arrayOfStates[r, c] = 0;  // obnulim sostoyania (0 - net bomby i riadom net bomb)
				}
		}
		void putMines()
		{ 
			int n = 0; // kolicestvo rasstavlennyh min  
			int r, c; // indeksy yaceek na pole
			System::Random^ rnd = (gcnew System::Random());   
			do 
			{ 
				r = rnd->Next(this->rows); 
				c = rnd->Next(this->colums); 
				if (this->arrayOfStates[r, c] != -1) 
				{ 
					this->arrayOfStates[r, c] = -1; 
					n++; 
				} 
			} 
			while (n != this->numberOfMines); 
		}

		void calculateNumbers()
		{
			for(int r = 0; r < rows; r++)
				for(int c = 0; c < colums; c++)
				{
					if(arrayOfStates[r, c] == -1) // esli v dannoy yaceyke bomba, to rasstaviti cisla vokrug
					{
						//if (r != rows -1)
						{
							if(c > 0) // esli ne vyshli za predely polia
								if (arrayOfStates[r, c - 1] != -1) arrayOfStates[r, c - 1]++; //shah vlevo
							if(c < rows - 1)
								if (arrayOfStates[r, c + 1] != -1) arrayOfStates[r, c + 1]++; // shag vpravo
							if(c > 0 && r > 0)
								if (arrayOfStates[r - 1, c - 1] != -1) arrayOfStates[r - 1, c - 1]++; // shag vlevo vverh
							if(r > 0)
								if (arrayOfStates[r - 1, c] != -1) arrayOfStates[r - 1, c]++; // shag vverh
							if(r > 0 && c < rows - 1)
								if (arrayOfStates[r - 1, c + 1] != -1) arrayOfStates[r - 1, c + 1]++; // shag vpravo vverh
							if(c > 0 && r < colums - 1)
								if (arrayOfStates[r + 1, c - 1] != -1) arrayOfStates[r + 1, c - 1]++; // shag vlevo vniz
							if(r < colums - 1)
								if (arrayOfStates[r + 1, c] != -1) arrayOfStates[r + 1, c]++; // shag vniz
							if(c < rows - 1 && r < colums - 1)
								if (arrayOfStates[r + 1, c + 1] != -1) arrayOfStates[r + 1, c + 1]++; //shag vpravo vniz
						}
					}
				}
		}
		void refreshField()
		{
			//ustanovim kolicestvo min v richTextBox-e	
			if(this->numberOfMines > 99)
				this->richTextBoxMines->Text = Convert::ToString(this->numberOfMines);
			else 
				this->richTextBoxMines->Text = "0" + Convert::ToString(this->numberOfMines);

			richTextBoxTime->Text = "000";

			for(int r = 0; r < rows; r++)
				for(int c = 0; c < colums; c++)
				{
					// esli proigrali to Click byl sniat, nado dobaviti obratno
					this->arrayOfButtons[r, c]->Click += gcnew System::EventHandler(this, &Form1::buttonOnField_Click);
					this->arrayOfButtons[r, c]->Enabled = true;
					this->arrayOfButtons[r, c]->BackColor = System::Drawing::Color::Gainsboro;
					this->arrayOfButtons[r, c]->Text = "";
					this->arrayOfButtons[r, c]->Image = nullptr;
					this->arrayOfButtons[r, c]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::buttonOnField_Mouse_Click);
					this->arrayOfStates[r, c] = 0;  // obnulim sostoyania (0 - net bomby i riadom net bomb)
				}
		}
		//raskraska knopki
		void paintButton(int r, int c)
		{
			this->arrayOfButtons[r, c]->BackColor = System::Drawing::Color::DarkBlue;
			switch(this->arrayOfStates[r, c])
			{
			case 1:  
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::Aquamarine;
				break;
			case 2: 
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::Orange;
				break;
			case 3:
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::DeepPink;
				break;
			case 4: 
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::OliveDrab;
				break;
			case 5:
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::GreenYellow;
				break;
			case 6:
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::FloralWhite;
				break;
			case 7:
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::Lime;
				break;
			case 8:
				this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::Red;
				break;
			}
			this->arrayOfButtons[r, c]->Text = Convert::ToString(this->arrayOfStates[r, c]);
			//zapret najatia		
			this->arrayOfButtons[r, c]->MouseDown -= gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::buttonOnField_Mouse_Click);
			this->arrayOfButtons[r, c]->Click -= gcnew System::EventHandler(this, &Form1::buttonOnField_Click);
		}
		// rekursivnyi metod otrkrytia knopok so znaceniem 0
		// r, c - indeksy       
		// vsegda staviti true!! Esli eto 1y metod v stack to on ostaetsia true, esli vyzvan iz sebia, to false
		void openFree(int r, int c, bool userClick)
		{
			if (this->arrayOfStates[r, c] != 0)
			{
				if (!userClick) return;
				if (this->arrayOfStates[r, c] == -1) 
				{ //esli nashli bombu
					for(int i = 0; i < rows; i++)
						for(int j = 0; j < colums; j++)
						{//otkryti bomby na pole
							if (Convert::ToInt32(arrayOfStates[i, j]) == -1)
							{//esli mina - to zagruziti izobrajenie miny v knopku
								this->arrayOfButtons[i, j]->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"mine")));
								this->arrayOfButtons[i, j]->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;
							}
							else
							{//esli ne pravilno flagi ponastavliali
								if(Convert::ToInt32(arrayOfStates[i, j]) != -1 && this->arrayOfButtons[i, j]->Image != nullptr)
								{
									this->arrayOfButtons[i, j]->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"notMine")));
									this->arrayOfButtons[i, j]->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;
									this->arrayOfButtons[i, j]->Text = "";
								}
							}
							//zapret myshy na pole
							this->arrayOfButtons[i, j]->Click -= gcnew System::EventHandler(this, &Form1::buttonOnField_Click);
							this->arrayOfButtons[i, j]->MouseDown -= gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::buttonOnField_Mouse_Click);

						}
						//pomeniati smile
						this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"looser")));
						this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::BottomCenter;
						stopTimer(); // ostanovka timera
						return;
				}
				return;
			}
			
			//zakrasiti najatuyu knopku
			this->arrayOfButtons[r, c]->Text = String::Empty;
			this->arrayOfButtons[r, c]->ForeColor = System::Drawing::Color::DarkBlue;
			//zapret najatia, esli ne flag na knopke
			if(this->arrayOfButtons[r, c]->Image == nullptr)
			{
				this->arrayOfButtons[r, c]->MouseDown -= gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::buttonOnField_Mouse_Click);
				this->arrayOfButtons[r, c]->Click -= gcnew System::EventHandler(this, &Form1::buttonOnField_Click);
			}

			arrayOfStates[r, c] = -2; //znacit my uje smotreli etu knopku

			for (int k = -1; k < 2; k++)
			{
				for (int l = -1; l < 2; l++)
				{
					int posR = r + k;
					int posC = c + l;
					if (l != 2 && k != 2 && posR >= 0 && posR < rows && posC >= 0 && posC < colums 
						&& this->arrayOfStates[posR, posC] != -1)
					{//pokrasim cyferki
						if(this->arrayOfButtons[posR, posC]->Image == nullptr)
						{
							this->arrayOfButtons[posR, posC]->BackColor = System::Drawing::Color::DarkBlue;
							if (this->arrayOfStates[posR, posC] != 0) 
							{
								paintButton(posR, posC);
								//zapret myshy na pole
								this->arrayOfButtons[posR, posC]->Click -= gcnew System::EventHandler(this, &Form1::buttonOnField_Click);
								this->arrayOfButtons[posR, posC]->MouseDown -= gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::buttonOnField_Mouse_Click);
							}
							else 
							{
								this->arrayOfButtons[posR, posC]->Text = String::Empty;
								this->arrayOfButtons[posR, posC]->ForeColor = System::Drawing::Color::DarkBlue;
								this->arrayOfButtons[posR, posC]->Image = nullptr;
							}
						}
						//esli esti eshe svobodnye knopocki
						if (this->arrayOfStates[posR, posC] == 0)
						{
							openFree(posR, posC, false);
						}
					}
				}
			}
	}
#pragma region Timer
		//zapusk timera na forme
		void runTimer()
		{
			timer->Interval = 1000;
			timer->Tick += gcnew System::EventHandler(this, &Form1::timer_Tick);
			timer->Start();
			timer->Enabled = true;
		}
		//ostanovka timera
		void stopTimer()
		{
			timer->Stop();
			timer->Enabled = false;
			timer->Tick -= gcnew System::EventHandler(this, &Form1::timer_Tick);
			timer->Interval = 1000;
		}
		//obrabotcik timera
		private: System::Void timer_Tick(System::Object^ sender, System::EventArgs^ e)
					{
						timer->Enabled = false;
						++time;
						if (time < 10) 
							richTextBoxTime->Text = "00" + Convert::ToString(time);
						else if (time < 100 && time >= 10)
							richTextBoxTime->Text = "0" + Convert::ToString(time);
						else 
							richTextBoxTime->Text = Convert::ToString(time);
						timer->Enabled = true;
					}
#pragma endregion
#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->gameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newGameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->easyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->normalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->championsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->abToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startPanel = (gcnew System::Windows::Forms::Panel());
			this->richTextBoxTime = (gcnew System::Windows::Forms::RichTextBox());
			this->richTextBoxMines = (gcnew System::Windows::Forms::RichTextBox());
			this->buttonStart = (gcnew System::Windows::Forms::Button());
			this->fieldPanel = (gcnew System::Windows::Forms::Panel());
			this->timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip->SuspendLayout();
			this->startPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->gameToolStripMenuItem, 
				this->championsToolStripMenuItem, this->abToolStripMenuItem});
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(311, 24);
			this->menuStrip->TabIndex = 0;
			this->menuStrip->Text = L"menuStrip";
			// 
			// gameToolStripMenuItem
			// 
			this->gameToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->newGameToolStripMenuItem, 
				this->toolStripSeparator1, this->easyToolStripMenuItem, this->normalToolStripMenuItem, this->hardToolStripMenuItem, this->toolStripSeparator2, 
				this->exitToolStripMenuItem});
			this->gameToolStripMenuItem->Name = L"gameToolStripMenuItem";
			this->gameToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->gameToolStripMenuItem->Text = L"Game";
			// 
			// newGameToolStripMenuItem
			// 
			this->newGameToolStripMenuItem->Name = L"newGameToolStripMenuItem";
			this->newGameToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->newGameToolStripMenuItem->Text = L"New Game";
			this->newGameToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::newGameToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
			// 
			// easyToolStripMenuItem
			// 
			this->easyToolStripMenuItem->Name = L"easyToolStripMenuItem";
			this->easyToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->easyToolStripMenuItem->Text = L"Easy";
			// 
			// normalToolStripMenuItem
			// 
			this->normalToolStripMenuItem->Name = L"normalToolStripMenuItem";
			this->normalToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->normalToolStripMenuItem->Text = L"Normal";
			// 
			// hardToolStripMenuItem
			// 
			this->hardToolStripMenuItem->Name = L"hardToolStripMenuItem";
			this->hardToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->hardToolStripMenuItem->Text = L"Hard";
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(149, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// championsToolStripMenuItem
			// 
			this->championsToolStripMenuItem->Name = L"championsToolStripMenuItem";
			this->championsToolStripMenuItem->Size = System::Drawing::Size(71, 20);
			this->championsToolStripMenuItem->Text = L"Champions";
			// 
			// abToolStripMenuItem
			// 
			this->abToolStripMenuItem->Name = L"abToolStripMenuItem";
			this->abToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->abToolStripMenuItem->Text = L"About";
			this->abToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::abToolStripMenuItem_Click);
			// 
			// startPanel
			// 
			this->startPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->startPanel->Controls->Add(this->richTextBoxTime);
			this->startPanel->Controls->Add(this->richTextBoxMines);
			this->startPanel->Controls->Add(this->buttonStart);
			this->startPanel->Location = System::Drawing::Point(9, 27);
			this->startPanel->Name = L"startPanel";
			this->startPanel->Size = System::Drawing::Size(294, 46);
			this->startPanel->TabIndex = 2;
			// 
			// richTextBoxTime
			// 
			this->richTextBoxTime->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->richTextBoxTime->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 17, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->richTextBoxTime->ForeColor = System::Drawing::SystemColors::WindowText;
			this->richTextBoxTime->Location = System::Drawing::Point(240, 4);
			this->richTextBoxTime->MaxLength = 3;
			this->richTextBoxTime->Multiline = false;
			this->richTextBoxTime->Name = L"richTextBoxTime";
			this->richTextBoxTime->ReadOnly = true;
			this->richTextBoxTime->Size = System::Drawing::Size(47, 36);
			this->richTextBoxTime->TabIndex = 2;
			this->richTextBoxTime->Text = L"000";
			// 
			// richTextBoxMines
			// 
			this->richTextBoxMines->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->richTextBoxMines->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 17, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->richTextBoxMines->ForeColor = System::Drawing::Color::Black;
			this->richTextBoxMines->Location = System::Drawing::Point(3, 3);
			this->richTextBoxMines->MaxLength = 3;
			this->richTextBoxMines->Multiline = false;
			this->richTextBoxMines->Name = L"richTextBoxMines";
			this->richTextBoxMines->ReadOnly = true;
			this->richTextBoxMines->Size = System::Drawing::Size(47, 36);
			this->richTextBoxMines->TabIndex = 1;
			this->richTextBoxMines->Text = L"000";
			// 
			// buttonStart
			// 
			this->buttonStart->Location = System::Drawing::Point(128, 1);
			this->buttonStart->Name = L"buttonStart";
			this->buttonStart->Size = System::Drawing::Size(41, 38);
			this->buttonStart->TabIndex = 0;
			this->buttonStart->UseVisualStyleBackColor = true;
			this->buttonStart->Click += gcnew System::EventHandler(this, &Form1::buttonStart_Click);
			// 
			// fieldPanel
			// 
			this->fieldPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->fieldPanel->Location = System::Drawing::Point(9, 79);
			this->fieldPanel->Name = L"fieldPanel";
			this->fieldPanel->Size = System::Drawing::Size(294, 294);
			this->fieldPanel->TabIndex = 3;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Desktop;
			this->ClientSize = System::Drawing::Size(311, 376);
			this->Controls->Add(this->fieldPanel);
			this->Controls->Add(this->startPanel);
			this->Controls->Add(this->menuStrip);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MineSweeper";
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->startPanel->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					this->Close();
				}

	private: System::Void buttonOnField_Click(System::Object^ sender, System::EventArgs^ e){	
					if(!timerFlag)//zapusk timera
						runTimer();
					timerFlag = true;
					//pomeniaem kartinku buttonStart
					this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"play")));
					this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;

					array<int>^ temp = gcnew array<int>(2);
					System::Windows::Forms::Button^ currentButton = ((System::Windows::Forms::Button^)sender);
					temp = (array<int>^)currentButton->Tag;
					if (this->arrayOfStates[temp[1], temp[0]] > 0)
					{
						paintButton(temp[1], temp[0]);
					}
					else
					{
						openFree(temp[1], temp[0], true);
					}

				}
	private: System::Void buttonStart_Click(System::Object^  sender, System::EventArgs^  e) {
					stopTimer();
					refreshField(); // vosstanavlivaet knopki na pole				
					startGame();
				}
	private: System::Void buttonOnField_Mouse_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
				{
					if(e->Button == System::Windows::Forms::MouseButtons::Right)
					{// esli najali pravuyu knopku myshy
						if(!timerFlag)//zapusk timera
							runTimer();
						timerFlag = true;
						
						//pomeniaem kartinku buttonStart
						this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"play")));
						this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;


						array<int>^ temp = gcnew array<int>(2);
						System::Windows::Forms::Button^ currentButton = ((System::Windows::Forms::Button^)sender);
						temp = (array<int>^)currentButton->Tag;
						//proveriaem esti uje flag ili net
						if (this->arrayOfButtons[temp[1], temp[0]]->Image == nullptr)
						{
							this->arrayOfButtons[temp[1], temp[0]]->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"myFlag")));
							this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;
							this->arrayOfButtons[temp[1], temp[0]]->Click -= gcnew System::EventHandler(this, &Form1::buttonOnField_Click);

							//pomeniaem kolicestvo min na pole v RichTextBoxMines
							if(this->numberOfMines > 99)
								this->richTextBoxMines->Text = Convert::ToString(Convert::ToInt32(this->richTextBoxMines->Text) - 1);
							else 
							{
								int var = Convert::ToInt32(this->richTextBoxMines->Text);
								if(var - 1 < -99)
								{
									this->richTextBoxMines->Text = "-00";
								}
								else
								{
									if(var - 1 < 0 )
										this->richTextBoxMines->Text = Convert::ToString(Convert::ToInt32(this->richTextBoxMines->Text) -1);
									else 
										this->richTextBoxMines->Text = "0" + Convert::ToString(Convert::ToInt32(this->richTextBoxMines->Text) -1);
								}
							}							
							if(this->arrayOfStates[temp[1], temp[0]] == -1)
								this->flagsOnMine++; // esli flag sverhu bomby

							//esli vse na vseh bombah flagi (i nigde bolee net flagov), to konec igry
							if(this->flagsOnMine == this->numberOfMines && Convert::ToInt32(richTextBoxMines->Text) == 0)
							{
								stopTimer(); // ostanovka timera
								this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"start")));
								this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;
								MessageBox::Show("Konec Igry!");
							}
						}
						else 
						{
							this->arrayOfButtons[temp[1], temp[0]]->Image = nullptr;
							this->arrayOfButtons[temp[1], temp[0]]->Click += gcnew System::EventHandler(this, &Form1::buttonOnField_Click);

							//pomeniaem kolicestvo min na pole v RichTextBoxMines
							if(this->numberOfMines > 99)
								this->richTextBoxMines->Text = Convert::ToString(Convert::ToInt32(this->richTextBoxMines->Text) + 1);
							else 
							{
								int var = Convert::ToInt32(this->richTextBoxMines->Text);
								if(var + 1 > this->numberOfMines)
								{
									this->richTextBoxMines->Text = Convert::ToString(this->numberOfMines);
								}
								else
								{
									if(var + 1 < -99)
										this->richTextBoxMines->Text = "-00";
									if(var + 1 > 0 )
										this->richTextBoxMines->Text = "0" + Convert::ToString(Convert::ToInt32(this->richTextBoxMines->Text) + 1);
									else 
										this->richTextBoxMines->Text = Convert::ToString(Convert::ToInt32(this->richTextBoxMines->Text) + 1);
								}
							}

							if(this->arrayOfStates[temp[1], temp[0]] == -1)
								this->flagsOnMine--; // esli flag byl sverhu bomby
							
							//esli vse na vseh bombah flagi (i nigde bolee net flagov), to konec igry
							if(this->flagsOnMine == this->numberOfMines && Convert::ToInt32(richTextBoxMines->Text) == 0)
							{
								stopTimer(); // ostanovka timera
								this->buttonStart->Image = (cli::safe_cast<System::Drawing::Image^  >(res->GetObject(L"start")));
								this->buttonStart->ImageAlign = System::Drawing::ContentAlignment::MiddleCenter;
								MessageBox::Show("Konec Igry!");
							}
						}
					}
				}
	private: System::Void abToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					MessageBox::Show("Rabotaet rejym \"Normal\"!\n \"Champions\" ne rabotaet (bylo leni uje :))", "About", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
				}
private: System::Void newGameToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					stopTimer();
					refreshField(); // vosstanavlivaet knopki na pole				
					startGame();
			}
};
}


