#include "gameloop.hpp"

#include "../Context/context.hpp"
#include "../Rendering/graphic.hpp"
#include "../Rendering/shader.hpp"
#include "../Rendering/texture.hpp"
#include "../Rendering/ImGUI/editor.hpp"
#include "../Input/input_responder.hpp"
#include "../Utils/miscellaneous.hpp"
#include "../Utils/time.hpp"
#include "../Utils/random_generator.hpp"


bool gameloop::run(int argc, char* argv[]) {

	std::filesystem::path exeFile = argv[0];
	environment::exeDirectory = exeFile.parent_path();
	environment::resourcesPath = environment::exeDirectory.parent_path() / "Resources";

	/*----------------------------------------------------------------------------------*/

	// Context

	context::initialiseGLFW();

	bool success = context::window::setupWindow("My Game", 600, 450);
	if (!success) {
		return false;
	}

	success = context::graphics::initialiseGraphics();
	if (!success) {
		return false;
	}

	context::imguiContext();

	/*----------------------------------------------------------------------------------*/

	{
		Shader shader(environment::ResourcePath("Shaders/shader.vs"), environment::ResourcePath("Shaders/shader.fs"));

		/*----------------------------------------------------------------------------------*/

		// Creating vertex data

		f32 vertices[] = {
		   -0.5f, -0.5f, -0.5f,  0.f, 0.f,
			0.5f, -0.5f, -0.5f,  1.f, 0.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
		   -0.5f,  0.5f, -0.5f,  0.f, 1.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 0.f,

		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,
			0.5f, -0.5f,  0.5f,  1.f, 0.f,
			0.5f,  0.5f,  0.5f,  1.f, 1.f,
			0.5f,  0.5f,  0.5f,  1.f, 1.f,
		   -0.5f,  0.5f,  0.5f,  0.f, 1.f,
		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,

		   -0.5f,  0.5f,  0.5f,  1.f, 0.f,
		   -0.5f,  0.5f, -0.5f,  1.f, 1.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,
		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,
		   -0.5f,  0.5f,  0.5f,  1.f, 0.f,

			0.5f,  0.5f,  0.5f,  1.f, 0.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
			0.5f, -0.5f, -0.5f,  0.f, 1.f,
			0.5f, -0.5f, -0.5f,  0.f, 1.f,
			0.5f, -0.5f,  0.5f,  0.f, 0.f,
			0.5f,  0.5f,  0.5f,  1.f, 0.f,

		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,
			0.5f, -0.5f, -0.5f,  1.f, 1.f,
			0.5f, -0.5f,  0.5f,  1.f, 0.f,
			0.5f, -0.5f,  0.5f,  1.f, 0.f,
		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,

		   -0.5f,  0.5f, -0.5f,  0.f, 1.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
			0.5f,  0.5f,  0.5f,  1.f, 0.f,
			0.5f,  0.5f,  0.5f,  1.f, 0.f,
		   -0.5f,  0.5f,  0.5f,  0.f, 0.f,
		   -0.5f,  0.5f, -0.5f,  0.f, 1.f
		};

		glm::vec3 cube_positions[] = {
			glm::vec3(-2.f, -2.f, -5.f),
			glm::vec3(0.f, -2.f, -5.f),
			glm::vec3(2.f, -2.f, -5.f),
			glm::vec3(-2.f, 0.f, -5.f),
			glm::vec3(0.f, 0.f, -5.f),
			glm::vec3(2.f, 0.f, -5.f),
			glm::vec3(-2.f, 2.f, -5.f),
			glm::vec3(0.f, 2.f, -5.f),
			glm::vec3(2.f, 2.f, -5.f),
		};

		std::vector<unsigned int> indices = {};

		/*----------------------------------------------------------------------------------*/

		// Sending Vertex data to GPU

		unsigned int VBO, VAO;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		VertexData vertex_data;

		vertex_data.setVBO(VBO);
		vertex_data.setVAO(VAO);

		vertex_data.bindVBO(vertices, sizeof(vertices), VBO);
		vertex_data.bindVAO(VAO);

		vertex_data.positionAttrib(0, 5 * sizeof(f32));
		vertex_data.textureAttrib(2, 5 * sizeof(f32));

		/*----------------------------------------------------------------------------------*/
		
		// Textures

		// Dropdown list of .txt file which contains all texture paths
		std::vector<std::string> textures = readFile(environment::ResourcePath("DirectoryReader/textures_list.txt"));

		// Initialise texture objects
		Texture transparent1(environment::ResourcePath("Textures/T_Transparent/graffiti_texture1.png"));

		auto texture1 = std::make_unique<Texture>(environment::ResourcePath("Textures/T_Metal/metal_bricks1.jpg"));
		auto texture2 = std::make_unique<Texture>(environment::ResourcePath("Textures/T_Wood/wood_planks1.jpg"));

		Texture error_texture(environment::ResourcePath("Textures/error_texture1.png"));

		// Add to a vector Texture unique ptrs
		std::vector<std::unique_ptr<Texture>> loaded_textures;

		loaded_textures.push_back(NULL);
		loaded_textures.push_back(std::move(texture1));
		loaded_textures.push_back(std::move(texture2));

		// Use shader
		shader.use();
		shader.setInt("transparent1", 1);

		/*----------------------------------------------------------------------------------*/

		// Cubes

		auto transform1 = std::make_unique<Transform>(0, cube_positions[0], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform2 = std::make_unique<Transform>(1, cube_positions[1], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform3 = std::make_unique<Transform>(2, cube_positions[2], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform4 = std::make_unique<Transform>(3, cube_positions[3], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform5 = std::make_unique<Transform>(4, cube_positions[4], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform6 = std::make_unique<Transform>(5, cube_positions[5], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform7 = std::make_unique<Transform>(6, cube_positions[6], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform8 = std::make_unique<Transform>(7, cube_positions[7], glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform9 = std::make_unique<Transform>(8, cube_positions[8], glm::quat(0.f, 1.0f, 0.3f, 0.5f));

		std::vector<std::unique_ptr<Transform>> cube_position_objects;

		cube_position_objects.push_back(std::move(transform1));
		cube_position_objects.push_back(std::move(transform2));
		cube_position_objects.push_back(std::move(transform3));
		cube_position_objects.push_back(std::move(transform4));
		cube_position_objects.push_back(std::move(transform5));
		cube_position_objects.push_back(std::move(transform6));
		cube_position_objects.push_back(std::move(transform7));
		cube_position_objects.push_back(std::move(transform8));
		cube_position_objects.push_back(std::move(transform9));

		/*----------------------------------------------------------------------------------*/

		Camera camera(-90.f, 0.f, (800.f / 2.f), (600.f / 2.f), 45.f, 100.f, 2.25f);

		Time last_frame = Time::now();

		bool should_isolte = false;

		/*----------------------------------------------------------------------------------*/

		// [TEMP] RNG Test: 

		int lower_bound_int = -500;
		int upper_bound_int = 500;
		int random_int = NDRNG::intInRange(lower_bound_int, upper_bound_int);
		std::cout << "Random integer in range (" << lower_bound_int << ", " << upper_bound_int << ") is: " << random_int << std::endl;

		std::cout << "\n";

		float lower_bound_float = -200.5f;
		float upper_bound_float = 200.5f;
		float random_float = NDRNG::floatInRange(lower_bound_float, upper_bound_float);
		std::cout << "Random float in range (" << lower_bound_float << ", " << upper_bound_float << ") is: " << random_float << std::endl;

		// [TEMP] RandomBatch Test:

		/*----------------------------------------------------------*/

		// DRNG 1

		RandomBatch DRNG1(3);

		std::cout << "\n\nDRNG1 : \n" << std::endl;
		DRNG1.debugOutput();

		/*----------------------------------------------------------*/

		// 1	[Current = 1; Next = 2;]

		u16 current1 = DRNG1.getCurrentIndex();
		u16 next1 = DRNG1.getNextIndex();
		s16 ran_i1 = DRNG1.intInRange(-100, 100);
		f32 ran_f1 = DRNG1.floatInRange(-50.5f, 50.5f);

		std::cout << "\n\n" << '1' << std::endl;
		std::cout << "Current Index: " << current1 << std::endl;
		std::cout << "Next: Index " << next1 << std::endl;
		std::cout << "Int random: " << ran_i1 << std::endl;
		std::cout << "Float random: " << ran_f1 << std::endl;

		/*----------------------------------------------------------*/

		// 2	[Current = 2; Next = 3;]

		DRNG1.moveForward();

		current1 = DRNG1.getCurrentIndex();
		next1 = DRNG1.getNextIndex();
		ran_i1 = DRNG1.intInRange(-100, 100);
		ran_f1 = DRNG1.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '2' << std::endl;
		std::cout << "Current: Index " << current1 << std::endl;
		std::cout << "Next: Index " << next1 << std::endl;
		std::cout << "Int random: " << ran_i1 << std::endl;
		std::cout << "Float random: " << ran_f1 << std::endl;

		/*----------------------------------------------------------*/

		// 3	[Current = 3; Next = 1;]

		DRNG1.moveForward();

		current1 = DRNG1.getCurrentIndex();
		next1 = DRNG1.getNextIndex();
		ran_i1 = DRNG1.intInRange(-100, 100);
		ran_f1 = DRNG1.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '3' << std::endl;
		std::cout << "Current: Index " << current1 << std::endl;
		std::cout << "Next: Index " << next1 << std::endl;
		std::cout << "Int random: " << ran_i1 << std::endl;
		std::cout << "Float random: " << ran_f1 << std::endl;

		/*----------------------------------------------------------*/

		// 4	[Current = 1; Next = 2;]

		DRNG1.moveForward();

		current1 = DRNG1.getCurrentIndex();
		next1 = DRNG1.getNextIndex();
		ran_i1 = DRNG1.intInRange(-100, 100);
		ran_f1 = DRNG1.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '4' << std::endl;
		std::cout << "Current: Index " << current1 << std::endl;
		std::cout << "Next: Index " << next1 << std::endl;
		std::cout << "Int random: " << ran_i1 << std::endl;
		std::cout << "Float random: " << ran_f1 << std::endl;

		/*----------------------------------------------------------*/

		// 5	[Current = 2; Next = 3;]

		DRNG1.moveForward();

		current1 = DRNG1.getCurrentIndex();
		next1 = DRNG1.getNextIndex();
		ran_i1 = DRNG1.intInRange(-100, 100);
		ran_f1 = DRNG1.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '5' << std::endl;
		std::cout << "Current: Index " << current1 << std::endl;
		std::cout << "Next: Index " << next1 << std::endl;
		std::cout << "Int random: " << ran_i1 << std::endl;
		std::cout << "Float random: " << ran_f1 << std::endl;

		/*----------------------------------------------------------*/

		// DRNG 2

		RandomBatch DRNG2(3);

		std::cout << "\n\nDRNG2 : \n" << std::endl;
		DRNG2.debugOutput();

		/*----------------------------------------------------------*/

		// 1	[Current = 1; Next = 2;]

		u16 current2 = DRNG2.getCurrentIndex();
		u16 next2 = DRNG2.getNextIndex();
		s16 ran_i2 = DRNG2.intInRange(-100, 100);
		f32 ran_f2 = DRNG2.floatInRange(-50.5f, 50.5f);

		std::cout << "\n\n" << '1' << std::endl;
		std::cout << "Current Index: " << current2 << std::endl;
		std::cout << "Next: Index " << next2 << std::endl;
		std::cout << "Int random: " << ran_i2 << std::endl;
		std::cout << "Float random: " << ran_f2 << std::endl;

		/*----------------------------------------------------------*/

		// 2	[Current = 2; Next = 3;]

		DRNG2.moveForward();

		current2 = DRNG2.getCurrentIndex();
		next2 = DRNG2.getNextIndex();
		ran_i2 = DRNG2.intInRange(-100, 100);
		ran_f2 = DRNG2.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '2' << std::endl;
		std::cout << "Current: Index " << current2 << std::endl;
		std::cout << "Next: Index " << next2 << std::endl;
		std::cout << "Int random: " << ran_i2 << std::endl;
		std::cout << "Float random: " << ran_f2 << std::endl;

		/*----------------------------------------------------------*/

		// 3	[Current = 3; Next = 1;]

		DRNG2.moveForward();

		current2 = DRNG2.getCurrentIndex();
		next2 = DRNG2.getNextIndex();
		ran_i2 = DRNG2.intInRange(-100, 100);
		ran_f2 = DRNG2.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '3' << std::endl;
		std::cout << "Current: Index " << current2 << std::endl;
		std::cout << "Next: Index " << next2 << std::endl;
		std::cout << "Int random: " << ran_i2 << std::endl;
		std::cout << "Float random: " << ran_f2 << std::endl;

		/*----------------------------------------------------------*/

		// 4	[Current = 1; Next = 2;]

		DRNG2.moveForward();

		current2 = DRNG2.getCurrentIndex();
		next2 = DRNG2.getNextIndex();
		ran_i2 = DRNG2.intInRange(-100, 100);
		ran_f2 = DRNG2.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '4' << std::endl;
		std::cout << "Current: Index " << current2 << std::endl;
		std::cout << "Next: Index " << next2 << std::endl;
		std::cout << "Int random: " << ran_i2 << std::endl;
		std::cout << "Float random: " << ran_f2 << std::endl;

		/*----------------------------------------------------------*/

		// 5	[Current = 2; Next = 3;]

		DRNG2.moveForward();

		current2 = DRNG2.getCurrentIndex();
		next2 = DRNG2.getNextIndex();
		ran_i2 = DRNG2.intInRange(-100, 100);
		ran_f2 = DRNG2.floatInRange(-50.5f, 50.5f);

		std::cout << "\n" << '5' << std::endl;
		std::cout << "Current: Index " << current2 << std::endl;
		std::cout << "Next: Index " << next2 << std::endl;
		std::cout << "Int random: " << ran_i2 << std::endl;
		std::cout << "Float random: " << ran_f2 << std::endl;

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);

			// Initialise DT
			Time delta_time = Time::now() - last_frame;
			last_frame = Time::now();

			camera.update(delta_time.getSeconds());

#ifdef DEBUG_MODE
			// Render imGUI
			context::createImguiWindow("My GUI###GUI1");

			if (ImGui::BeginTabBar("###tab_bar1")) {

				if (ImGui::BeginTabItem("Debug###debug1")) {
					collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, false);
					collapsingHeader::colour(false);
					collapsingHeader::miscellaneous(false);

					ImGui::TextWrapped("\n");

					ImGui::TextWrapped("\Ping: %lf", delta_time.getMilliseconds());
					ImGui::TextWrapped("\nFPS: %lf", getFramesPerSecond(delta_time.getSeconds()));

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Editor###editor1")) {
					ImGui::TextWrapped("\n");

					collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, true);
					collapsingHeader::colour(true);
					collapsingHeader::miscellaneous(true);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Help###help1")) {
					collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, false);
					collapsingHeader::colour(false);
					collapsingHeader::miscellaneous(false);

					ImGui::TextWrapped("\n");

					collapsingHeader::controlsText(true);
					collapsingHeader::aboutText(true);

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			else {
				collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, true);
				collapsingHeader::colour(true);
				collapsingHeader::miscellaneous(true);
			}

			should_isolte = isMouseOverUI();

			ImGui::End();
#endif

			// Projection + View + Transform [MATRICES]
			glm::mat4 projection = camera.getMat4Projection();
			shader.setMat4("projection", projection);

			glm::mat4 view = camera.getMat4View();
			shader.setMat4("view", view);

			glm::mat4 transform = camera.getMat4Transform();
			u16 transform_loc = glGetUniformLocation(shader.mID, "transform");
			shader.modMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Renders boxes
			for (size_t i = 0; i < cube_position_objects.size(); i++) {
				glm::mat4 model = cube_position_objects[i]->getModel();
				shader.setMat4("model", model);

				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// Unbind all texture units 
			std::vector<u16> texture_units = { 0, 1, 2 };
			Texture::unbind(texture_units);


#ifdef DEBUG_MODE
			context::renderImgui();
#endif
			context::window::swapBuffers();
			context::window::pollEvents();
			update(delta_time.getSeconds(), camera, should_isolte);
			input::endFrame();
		}
	}

	glfwTerminate();
	return true;
}
