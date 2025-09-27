module;

#include <SDL3/SDL_gpu.h>
#include <filesystem>
#include <string_view>

export module druid.graphics.renderer;

import druid.graphics.pipeline;

#if _WIN32 || __linux__ || __ANDROID__
constexpr auto ShaderFormat = SDL_GPUShaderFormat{SDL_GPU_SHADERFORMAT_SPIRV};
constexpr auto ShaderEntryPoint = "main";
constexpr auto ShaderExt = ".spv";
#elif __APPLE__
constexpr auto ShaderFormat = SDL_GPUShaderFormat{SDL_GPU_SHADERFORMAT_MSL};
constexpr auto ShaderEntryPoint = "main0";
constexpr auto ShaderExt = ".msl";
#elif __EMSCRIPTEN__
#else
#endif

namespace druid::graphics
{
	auto ShaderExtension(std::string_view x) -> std::filesystem::path
	{
		auto path = std::filesystem::path{x};
		path += ShaderExt;
		return path;
	}

	export class Renderer
	{
	public:
		Renderer(SDL_Window& x) : window{&x}, device{SDL_CreateGPUDevice(ShaderFormat, false, nullptr)}
		{
			SDL_ClaimWindowForGPUDevice(this->device, this->window);
		}

		~Renderer()
		{
			SDL_ReleaseWindowFromGPUDevice(this->device, this->window);
			SDL_DestroyGPUDevice(this->device);
		}

		Renderer(const Renderer&) = delete;
		auto operator=(const Renderer&) -> Renderer& = delete;

		Renderer(Renderer&&) noexcept = default;
		auto operator=(Renderer&&) noexcept -> Renderer& = default;

		[[nodiscard]] auto handle() const noexcept -> SDL_GPUDevice*
		{
			return this->device;
		}

		[[nodiscard]] auto createPipeline() const -> std::unique_ptr<Pipeline>
		{
			const std::filesystem::path shaderDir{SHADER_DIR};
			Pipeline::Description desc{};
			desc.device = this->device;
			desc.vertex = shaderDir / ShaderExtension("default.vert");
			desc.fragment = shaderDir / ShaderExtension("default.frag");
			desc.entryPoint = ShaderEntryPoint;
			desc.format = ShaderFormat;

			SDL_GPUColorTargetDescription colorDesc{};
			colorDesc.format = SDL_GetGPUSwapchainTextureFormat(this->device, this->window);
			desc.desc = colorDesc;

			return std::make_unique<Pipeline>(desc);
		}

	private:
		SDL_Window* window{};
		SDL_GPUDevice* device{};
	};
}