#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

#include <vector>

class Button
{
private:
	int x; int y;
	string buttonSrc;
public:
	Button()
		: x(0), y(0), buttonSrc("On_Button.png") { }

	Button(const int _x, const int _y, string _buttonSrc)
		: x(_x), y(_y), buttonSrc(_buttonSrc) { }

	Mat imgButton = imread(buttonSrc);
	Rect rectButton = Rect(x, y, imgButton.cols, imgButton.rows);
};

class CheckBox
{
private:
	int x; int y;
public:
	string checkBoxOnSrc; string checkBoxOffSrc;

	CheckBox()
		:x(0), y(0), checkBoxOnSrc("CheckBoxON.png"), checkBoxOffSrc("CheckBoxOFF.png") { }

	CheckBox(const int _x, const int _y, string _checkBoxOnSrc, string _checkBoxOffSrc)
		:x(_x), y(_y), checkBoxOnSrc(_checkBoxOnSrc), checkBoxOffSrc(_checkBoxOffSrc) { }

	bool checkBoxFlag = true;

	Mat imgCheckBox = imread(checkBoxOnSrc);
	Rect rectCheckBox = Rect(x, y, imgCheckBox.cols, imgCheckBox.rows);
};

class Mouse
{
public:
	void myMouseCallback(int event, int x, int y, int flags, void* param);
};

Mouse* g_ptr;
void onMouse(int event, int x, int y, int flags, void* param)
{
	g_ptr->myMouseCallback(event, x, y, flags, param);
}

class Manager
{
public:
	vector<Button> buttons;
	vector<CheckBox> checkboxes;

	Mouse mouseCallBack;
	Mat canvas = Mat(300, 300, CV_8UC3, Scalar(0, 0, 0));

	Manager() { g_ptr = &mouseCallBack; }

	void AddButton(Button button)
	{
		buttons.push_back(button);
		buttons.back().imgButton.copyTo(canvas(buttons.back().rectButton));
	}

	void AddCheckbox(CheckBox checkbox)
	{
		checkboxes.push_back(checkbox);
		checkboxes.back().imgCheckBox.copyTo(canvas(checkboxes.back().rectCheckBox));
	}
};

// обработчик событий от мышки
void Mouse::myMouseCallback(int event, int x, int y, int flags, void* param)
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
						(*manager).checkboxes[n].imgCheckBox = imread((*manager).checkboxes[n].checkBoxOffSrc);
						(*manager).checkboxes[n].imgCheckBox.copyTo((*manager).canvas((*manager).checkboxes[n].rectCheckBox));
					}
					else
					{
						(*manager).checkboxes[n].checkBoxFlag = true;
						(*manager).checkboxes[n].imgCheckBox = imread((*manager).checkboxes[n].checkBoxOnSrc);
						(*manager).checkboxes[n].imgCheckBox.copyTo((*manager).canvas((*manager).checkboxes[n].rectCheckBox));
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
	const string buttonSrc = "On_Button.png";
	const string checkBoxOnSrc = "CheckBoxON.png"; const string checkBoxOffSrc = "CheckBoxOFF.png";

	Manager manager;

	manager.AddButton(Button(0, 0, buttonSrc));
	manager.AddButton(Button(100, 0, buttonSrc));
	manager.AddCheckbox(CheckBox(0, 100, checkBoxOnSrc, checkBoxOffSrc));
	manager.AddCheckbox(CheckBox(100, 100, checkBoxOnSrc, checkBoxOffSrc));

	void *interfaceParam = &manager;

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);

	// вешаем обработчик мышки
	cvSetMouseCallback("original", onMouse, interfaceParam);

	while (1) {
		// показываем картинку
		imshow("original", manager.canvas);

		char c = cvWaitKey(33);
		if (c == 27) { // если нажата ESC - выходим
			break;
		}
	}

	// удаляем окно
	cvDestroyWindow("original");
	return 0;
}