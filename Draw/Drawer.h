#pragma once

#include <afxwin.h>
#include <vector>

using namespace std;

class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, области рисования.
	CWnd* wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC* dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;
	// Флаг для отслеживания состояния инициализации класса.
	bool init;
public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd)
	{
		// Получаем указатель на окно.
		wnd = CWnd::FromHandle(hWnd);
		// Получаем прямоугольник окна.
		wnd->GetClientRect(frame);
		// Получаем контекст для рисования в этом окне.
		dc = wnd->GetDC();

		// Создаем буфер-контекст.
		memDC.CreateCompatibleDC(dc);
		// Создаем растр для контекста рисования.
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		// Выбираем растр для использования буфер-контекстом.
		memDC.SelectObject(&bmp);
		init = true;
	}

	// Нарисовать график по переданным данным.
	void Draw(vector<double>& y_first_graph, vector<double>& y_second_graph, vector<double>& y_ellipse,
		vector<double>& x_first_graph = vector<double>(), vector<double>& x_second_graph = vector<double>(), vector<double>& x_ellipse = vector<double>())
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_pen2(PS_SOLID, 2, RGB(38, 0, 255));
		CPen data_pen3(PS_SOLID, 2, RGB(128, 0, 128));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Курсив
			FALSE,				// Подчеркнутый
			FALSE,				// Перечеркнутый
			DEFAULT_CHARSET,	// Набор символов
			OUT_OUTLINE_PRECIS,	// Точность соответствия.	
			CLIP_DEFAULT_PRECIS,//  
			100,	// Качество
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		// Рисуем график.
		if (y_first_graph.empty()) return;

		if (x_first_graph.size() != y_first_graph.size())
		{
			x_first_graph.resize(y_first_graph.size());
			for (int i = 0; i < x_first_graph.size(); i++)
			{
				x_first_graph[i] = i;
			}
		}

		memDC.SelectObject(&data_pen);

		double data_y_max(0.6), data_y_min(-0.8);
		double data_x_max(2), data_x_min(-2);

		vector<double> y = convert_range(y_first_graph, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(x_first_graph, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		//////////////////////////////////////////////////////////////////////////
		//Рисуем графики 2.0
		// Рисуем график.
		if (y_second_graph.empty()) return;

		if (x_second_graph.size() != y_second_graph.size())
		{
			x_second_graph.resize(y_second_graph.size());
			for (int i = 0; i < x_second_graph.size(); i++)
			{
				x_second_graph[i] = i;
			}
		}

		memDC.SelectObject(&data_pen2);

		vector<double> y2 = convert_range(y_second_graph, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x2 = convert_range(x_second_graph, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x2[0], y2[0]);
		for (unsigned int i = 0; i < y2.size(); i++)
		{
			memDC.LineTo(x2[i], y2[i]);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		//////////////////////////////////////////////////////////////////////////
	// Рисуем ellipse.
		if (y_ellipse.empty()) return;

		if (x_ellipse.size() != y_ellipse.size())
		{
			x_ellipse.resize(y_ellipse.size());
			for (int i = 0; i < x_ellipse.size(); i++)
			{
				x_ellipse[i] = i;
			}
		}

		memDC.SelectObject(&data_pen3);

		vector<double> y3 = convert_range(y_ellipse, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x3 = convert_range(x_ellipse, actual_right, actual_left, data_x_max, data_x_min);
		for (unsigned int i = 0; i < y3.size(); i++)
		{
			//memDC.MoveTo(x3[i], y3[i]);
			memDC.Ellipse(x3[i]-3, y3[i]-3, x3[i]+6, y3[i] + 6);//, x3[i]+0.1, y3[i]+0.1);
		}
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		//x3[i] - 0.5, y3[i] + 0.5, x3[i] + 0.5, y3[i] - 0.5
	}
	//////////////////////////////////////////////////////////////////////////
	

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
	{
		vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
};