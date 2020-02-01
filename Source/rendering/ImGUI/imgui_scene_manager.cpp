#include "imgui_scene_manager.h"



void createImguiWindow(std::string name) {
	// Feed input to imGUI, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(name.data());
}

/*----------------------------------------------------------------------------------*/

CreateButton::CreateButton(std::string name, std::string ID)
	: mName(name), mID(ID), mClicked(false), mToggledClick(false) {

	static int click_amount = 0;

	ImGui::Text(mName.data());
	if (ImGui::Button(mID.data())) {
		mClicked = true;

		click_amount++;
	}

	if (click_amount % 2) {
		mToggledClick = true;
	}
}

const bool CreateButton::isClicked() {
	return mClicked;
}

const bool CreateButton::isToggledClick() {
	return mToggledClick;
}

/*----------------------------------------------------------------------------------*/

CreateCheckbox::CreateCheckbox(std::string name, std::string ID)
	: mName(name), mID(ID), mChecked(false) {

	static bool checked = false;

	ImGui::Text(mName.data());
	ImGui::Checkbox(mID.data(), &checked);

	checkboxLogic(checked);
}

void CreateCheckbox::checkboxLogic(const bool& checked) {
	if (checked) {
		mChecked = true;
	}
}

const bool CreateCheckbox::isChecked() {
	return mChecked;
}

/*----------------------------------------------------------------------------------*/

CreateSlider::CreateSlider(std::string name, std::string ID, int begin, int end)
	 :mName(name), mID(ID) {

	static int slider_value = 0;

	ImGui::Text(mName.data());
	ImGui::SliderInt(mID.data(), &slider_value, begin, end);

	mValueInt = slider_value;
}

CreateSlider::CreateSlider(std::string name, std::string ID, float begin, float end) 
	:mName(name), mID(ID) {

	static float slider_value = 0.0f;
	
	ImGui::Text(mName.data());
	ImGui::SliderFloat(mID.data(), &slider_value, begin, end);

	mValueFloat = slider_value;
}

const int CreateSlider::getIntSliderValue() {
	return mValueInt;
}

const float CreateSlider::getFloatSliderValue() {
	return mValueFloat;
}

/*----------------------------------------------------------------------------------*/

CreateTextReader::CreateTextReader(std::string name, std::string ID, unsigned int count)
	: mName(name), mID(ID) {

	static char input_text[] = "";

	ImGui::Text(mName.data());
	ImGui::InputText(mID.data(), input_text, count + 1);

	mText = input_text;
}

const std::string CreateTextReader::getText() {
	return mText;
}

/*----------------------------------------------------------------------------------*/

CreateListBox::CreateListBox(std::string name, std::string ID, std::vector<std::string> list_of_items, unsigned int height)
	: mName(name), mID(ID) {

	std::vector<const char*> items;
	for (size_t i = 0; i < list_of_items.size(); i++) {
		items.push_back(list_of_items[i].data());
	}

	static int selected_item = 0;

	ImGui::Text(mName.data());
	ImGui::ListBox(mID.data(), &selected_item, items.data(), items.size(), height);

	mSelectedItem = selected_item;
}

const int CreateListBox::getSelectedItem() {
	return mSelectedItem;
}

/*----------------------------------------------------------------------------------*/

CreateCombo::CreateCombo(std::string name, std::string ID, std::vector<std::string> list_of_items, unsigned int height)
	: mName(name), mID(ID) {

	std::vector<const char*> items;
	for (size_t i = 0; i < list_of_items.size(); i++) {
		items.push_back(list_of_items[i].data());
	}

	static int selected_item = 0;

	ImGui::Text(mName.data());
	ImGui::Combo(mID.data(), &selected_item, items.data(), items.size(), height);

	mSelectedItem = selected_item;
}

const int CreateCombo::getSelectedItem() {
	return mSelectedItem;
}

/*----------------------------------------------------------------------------------*/

