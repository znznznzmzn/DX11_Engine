#pragma once

namespace EditorGUI {
	const string filter_image = "Imagefiles{.tga,.png,.gif,.jpg,.jpeg}";
	bool ImageButton(const string& title, Texture*& img, ImVec2 size = { 80, 80 });
	bool ImageButton_Dialog_LoadTexture(const string& key, const string& title, Texture*& img, ImVec2 size = { 80, 80 });
}