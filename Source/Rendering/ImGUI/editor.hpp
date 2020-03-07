#include <imgui.h>
#include "Rendering/ImGUI/imgui_impl_glfw.hpp"
#include "Rendering/ImGUI/imgui_impl_opengl3.hpp"

#include "Utils/miscellaneous.hpp"
#include "Rendering/graphic.hpp"
#include "Rendering/texture.hpp"
#include "Input/input_responder.hpp"


#ifdef DEBUG_MODE
	namespace collapsingHeader {

		void texture(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const std::shared_ptr<Texture> error_texture, const std::shared_ptr<Texture> transparent_texture, bool editor);

		class TextureUI {
		private:
			std::vector<std::string> mTextures;
			std::vector<std::unique_ptr<Texture>> mLoadedTextures;
			std::shared_ptr<Texture> mErrorTexture;
			std::shared_ptr<Texture> mTransparentTexture;

			const char* mCurrentItem;
			u8 mSelectedValue;

		public:
			//TextureUI(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const std::shared_ptr<Texture> error_texture, const std::shared_ptr<Texture> transparent_texture);

			static bool mApplyTexture;
			static bool mApplyTransparent;

			void display();
			void process();
		};

		class ColourUI {
		public:
			static ImVec4 colour;
			static bool apply_background;

			void display();
			void process();
		};

		class MiscellaneousUI {
		public:
			static bool mWireframeModeChecked;

			void display();
			void process();
		};

		void controlsText();
		void aboutText();
	}

	bool isMouseOverUI();
#endif
