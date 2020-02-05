#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void createImguiWindow(std::string name);

/*----------------------------------------------------------------------------------*/

class CreateButton {										//  USAGE:
private:
	std::string mName;										//  CreateButton button1("My Button: ", "Press this!###button1");		
	std::string mID;										//  if (button1.isClicked()) {		
	bool mClicked;											//  	// [DO STUFF]	
	bool mToggledClick;									    //  }		
															//  if (button1.isToggledClick()) {		
public:														//  	// [DO STUFF]		
	CreateButton(std::string name, std::string ID);			//  }		

	const bool isClicked();												
	const bool isToggledClick();
};

/*----------------------------------------------------------------------------------*/

class CreateCheckbox {										// USAGE:				
private:
	std::string mName;										//  CreateCheckbox checkbox1("\nMy Checkbox: ", "###checkbox1");
	std::string mID;										//  if (checkbox1.isChecked()) {
	static bool mChecked;									//		// [DO STUFF]
															//  }
public:
	CreateCheckbox(std::string name, std::string ID);

	void tickCheckbox();
	void untickCheckbox();
	void reverseCheckbox();

	const bool isChecked();
};

/*----------------------------------------------------------------------------------*/

class CreateSlider {										//USAGE: 
private:
	std::string mName;										//  CreateSlider int_slider1("\nMy Slider: ", "###slider_int1", 0, 100);
	std::string mID;										//  int slider_value_int = int_slider1.getIntSliderValue();
	int mValueInt;											//	CreateSlider float_slider1("\nMy Slider: ", "###slider_float1", 0.0f, 100.0f);
	float mValueFloat;										//	float slider_value_float = float_slider1.getFloatSliderValue();
															//  [ACT ACCORDING TO VALUES]
public:
	explicit CreateSlider(std::string name, std::string ID, int begin, int end);
	explicit CreateSlider(std::string name, std::string ID, float begin, float end);

	const int getIntSliderValue();
	const float getFloatSliderValue();
};

/*----------------------------------------------------------------------------------*/

class CreateTextReader {									//USAGE: 																					
private:
	std::string mName;										//  CreateTextReader text_reader("\nInput Text: ", "###input_text1", 10);
	std::string mID;										//  std::string text = text_reader.getText();
	std::string mText;										//  [ACT ACCORDING TO VALUES]

public:
	CreateTextReader(std::string name, std::string ID, unsigned int count);

	const std::string getText();
};

/*----------------------------------------------------------------------------------*/

class CreateListBox {										//USAGE: 													
private:
	std::string mName;										//  std::vector<std::string> items = { "One", "Two", "Three" };
	std::string mID;										//	CreateListBox listbox1("\nMy ListBox: ", "###listbox1", items, 3);
	int mSelectedItem;										//  int selected_item = listbox1.getSelectedItem();
															// [ACT ACCORDING TO VALUES]
public:
	CreateListBox(std::string name, std::string ID, std::vector<std::string> list_of_items, unsigned int height);

	const int getSelectedItem();
};

/*----------------------------------------------------------------------------------*/

class CreateCombo {											//USAGE:													
private:
	std::string mName;										//  std::vector<std::string> items = { "One", "Two", "Three" };
	std::string mID;										//	CreateCombo combo1("\nMy Combo: ", "###combo1", items, 3);
	int mSelectedItem;										//	int selected_item = combo1.getSelectedItem();
															//	[ACT ACCORDING TO VALUES]
public:
	CreateCombo(std::string name, std::string ID, std::vector<std::string> list_of_items, unsigned int height);
	const int getSelectedItem();
};

/*----------------------------------------------------------------------------------*/
