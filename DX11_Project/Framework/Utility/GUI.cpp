#include "Framework.h"

namespace EditorGUI {
	bool ImageButton(const string& title, Texture*& img, ImVec2 size) {
		return ((img != nullptr) ?
			ImGui::ImageButton(img->GetSRV(), { size.x - 8, size.y - 8 }) :
			ImGui::Button(title.c_str(), size));
	}
	bool ImageButton_Dialog_LoadTexture(const string& key, const string& title, Texture*& img, ImVec2 size) {
		if (ImageButton(title, img, size))
			DIALOG->OpenDialog(key, title, filter_image.c_str(), ".");

		if (DIALOG->Display(key)) {
			if (DIALOG->IsOk()) {
				string file = DIALOG->GetFilePathName();
				file = file.substr(projectPath.length() + 1, file.length());
				img = Texture::Add(ToWString(file));
				DIALOG->Close();
				return true;
			}
			DIALOG->Close();
		}
		return false;
	}
	/* »ı°¢
	bool Dialog(const string& key, Event evt, ImVec2 size = { 200, 200 }) {
		if (DIALOG->Display(key, 32, size)) {
			if (DIALOG->IsOk()) {
				evt();
				DIALOG->Close();
				return true;
			}
			DIALOG->Close();
		}
		return false;
	}*/
}
