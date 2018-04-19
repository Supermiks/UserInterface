#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

#include <iostream>
#include <vector>

class Button
{
private:
	int x; int y;
	string buttonSrc;
public:
	string  buttonFunctionFlag;
	string buttonColor;

	Button()
		: x(0), y(0), buttonSrc("On_Button2.png") { }

	Button(const int _x, const int _y, string _buttonSrc, string _buttonFlag, string _buttonColor)
		: x(_x), y(_y), buttonSrc(_buttonSrc), buttonFunctionFlag(_buttonFlag), buttonColor(_buttonColor){ }

	Mat imgButton = imread(buttonSrc, IMREAD_UNCHANGED);
	Rect rectButton = Rect(x, y, imgButton.cols, imgButton.rows);
};

class CheckBox
{
private:
	int x; int y;
public:
	string checkBoxOnSrc; string checkBoxOffSrc;
	string checkBoxFunctionFlag;

	CheckBox()
		:x(0), y(0), checkBoxOnSrc("CheckBoxON.png"), checkBoxOffSrc("CheckBoxOFF.png") { }

	CheckBox(const int _x, const int _y, string _checkBoxOnSrc, string _checkBoxOffSrc, string _checkBoxFlag)
		:x(_x), y(_y), checkBoxOnSrc(_checkBoxOnSrc), checkBoxOffSrc(_checkBoxOffSrc), checkBoxFunctionFlag(_checkBoxFlag){ }

	bool checkBoxFlag = true;

	Mat imgCheckBox = imread(checkBoxOnSrc, IMREAD_UNCHANGED);
	Rect rectCheckBox = Rect(x, y, imgCheckBox.cols, imgCheckBox.rows);
};

class Manager
{
public:
	vector<Button> buttons;
	vector<CheckBox> checkboxes;

	Mat interfaceImg = imread("Interface.png", IMREAD_UNCHANGED);
	Mat backup;
	Mat canvas;

	bool colorFlag = true;

	Manager() {}

	void AddCanvas(string canvasSrc)
	{
		canvas = imread(canvasSrc, IMREAD_UNCHANGED);
	}

	void AddButton(Button button)
	{
		buttons.push_back(button);
		buttons.back().imgButton.copyTo(interfaceImg(buttons.back().rectButton));
	}

	void AddCheckbox(CheckBox checkbox)
	{
		checkboxes.push_back(checkbox);
		checkboxes.back().imgCheckBox.copyTo(interfaceImg(checkboxes.back().rectCheckBox));
	}

	void Save()
	{
		while (1)
		{
			cout << "Enter the file name:" << endl;

			string canvasSrc;
			cin >> canvasSrc;

			try 
			{
				imwrite("canvasSrc", canvas);
			}
			catch (runtime_error& ex)
			{
				fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
				return;
			}

			cout << "Saved";
			break;
		}
	}

	void Load()
	{
		while (1)
		{
			cout << "Enter the file name:" << endl;

			string canvasSrc;
			cin >> canvasSrc;

			canvas = imread(canvasSrc, IMREAD_UNCHANGED);
			if (!canvas.data)                              // Check for invalid input
			{
				cout << "Could not open or find the image" << endl;
			}
			else
			{
				break;
			}
		}
	}

	void Color(string colorName)
	{
		int channel;

		if (colorName == "blue") { channel = 0;}
		else if (colorName == "green") { channel = 1; }
		else if (colorName == "red") { channel = 2; }
		else { channel = 0; }

		for (int i = 0; i < canvas.rows; i++)
		{
			for (int j = 0; j < canvas.cols; j++)
			{
				if (colorFlag)
				{
					canvas.at<Vec3b>(i, j)[channel] += 255 / 10;
				}
				else
				{
					canvas.at<Vec3b>(i, j)[channel] -= 255 / 10;
				}
			}
		}
	}

	void Grid(int color)
	{
		backup = canvas.clone();
		for (int i = 0; i < canvas.rows; i++)
		{
			for (int j = 0; j < canvas.cols; j++)
			{
				if ((i % 20 == 10 && j % 2 == 1) || (j % 50 == 25 && i % 2 == 1))
				{
					canvas.at<Vec3b>(i, j)[0] = color;
					canvas.at<Vec3b>(i, j)[1] = color;
					canvas.at<Vec3b>(i, j)[2] = color;
				}
			}
		}
	}
};

// обработчик событий от мышки
void myMouseCallback(int event, int x, int y, int flags, void* param)
{
	Manager *manager = (Manager*)param;

	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		break;

	case CV_EVENT_LBUTTONDOWN:
		if (event == EVENT_LBUTTONDOWN)
		{
			for (int n = 0; n < (*manager).buttons.size(); n++)
			{
				if ((*manager).buttons[n].rectButton.contains(Point(x, y)))
				{
					cout << "Button " << n + 1 << " clicked!" << endl;
					
					if ((*manager).buttons[n].buttonFunctionFlag == "Color")
					{
						(*manager).Color((*manager).buttons[n].buttonColor);
					}

					if ((*manager).buttons[n].buttonFunctionFlag == "Save")
					{
						(*manager).Save();
					}

					if ((*manager).buttons[n].buttonFunctionFlag == "Load")
					{
						(*manager).Load();
					}
				}
			}

			for (int n = 0; n < (*manager).checkboxes.size(); n++)
			{
				if ((*manager).checkboxes[n].rectCheckBox.contains(Point(x, y)))
				{
					cout << "CheckBox " << n + 1 << " clicked!" << endl;

					if ((*manager).checkboxes[n].checkBoxFlag)
					{
						(*manager).checkboxes[n].checkBoxFlag = false;
						(*manager).checkboxes[n].imgCheckBox = imread((*manager).checkboxes[n].checkBoxOffSrc, IMREAD_UNCHANGED);
						(*manager).checkboxes[n].imgCheckBox.copyTo((*manager).interfaceImg((*manager).checkboxes[n].rectCheckBox));

						if ((*manager).checkboxes[n].checkBoxFunctionFlag == "grid")
						{
							(*manager).Grid(255);
						}

						if ((*manager).checkboxes[n].checkBoxFunctionFlag == "color")
						{
							(*manager).colorFlag = false;
						}
					}
					else
					{
						(*manager).checkboxes[n].checkBoxFlag = true;
						(*manager).checkboxes[n].imgCheckBox = imread((*manager).checkboxes[n].checkBoxOnSrc, IMREAD_UNCHANGED);
						(*manager).checkboxes[n].imgCheckBox.copyTo((*manager).interfaceImg((*manager).checkboxes[n].rectCheckBox));

						if ((*manager).checkboxes[n].checkBoxFunctionFlag == "grid")
						{
							(*manager).canvas = (*manager).backup.clone();
						}

						if ((*manager).checkboxes[n].checkBoxFunctionFlag == "color")
						{
							(*manager).colorFlag = true;
						}
					}
				}
			}

			printf("%d x %d\n", x, y);
			break;
		}

	case CV_EVENT_LBUTTONUP:
		break;
	}
}

int main(int argc, char* argv[])
{
	const string redButtonSrc = "RedButton.png";
	const string greenButtonSrc = "GreenButton.png";
	const string blueButtonSrc = "BlueButton.png";
	const string saveSrc = "Save.png";
	const string loadSrc = "Load.png";
	const string checkBoxOnSrc = "CheckBoxON.png"; const string checkBoxOffSrc = "CheckBoxOFF.png";
	const string canvasSrc = "canvas.png";

	Manager manager;

	manager.AddCanvas(canvasSrc);

	//manager.AddButton(Button(75, 120, purpleButtonSrc, "Color", "purple"));
	manager.AddButton(Button(75, 180, blueButtonSrc, "Color", "blue"));
	manager.AddButton(Button(30, 150, redButtonSrc, "Color", "red"));
	manager.AddButton(Button(120, 150, greenButtonSrc, "Color", "green"));

	manager.AddButton(Button(25, 300, saveSrc, "Save", "None"));
	manager.AddButton(Button(115, 300, loadSrc, "Load", "None"));

	manager.AddCheckbox(CheckBox(25, 66, checkBoxOnSrc, checkBoxOffSrc, "color"));
	manager.AddCheckbox(CheckBox(125, 66, checkBoxOnSrc, checkBoxOffSrc, "grid"));

	void *interfaceParam = &manager;

	// окно для отображения картинки
	cvNamedWindow("interface", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("canvas", CV_WINDOW_AUTOSIZE);

	// вешаем обработчик мышки
	cvSetMouseCallback("interface", myMouseCallback, interfaceParam);

	while (1) {
		// показываем картинку
		imshow("interface", manager.interfaceImg);
		imshow("canvas", manager.canvas);

		char c = cvWaitKey(33);
		if (c == 27) { // если нажата ESC - выходим
			break;
		}
	}

	// удаляем окно
	cvDestroyWindow("original");
	return 0;
}