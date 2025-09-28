#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <cstdlib>

#include <SDL3/SDL.h>

#include <chrono>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>

import druid.core.file;
import druid.graphics.color;
import druid.graphics.vertex;
import druid.graphics.window;
import druid.graphics.event;

using druid::graphics::Color;
using druid::graphics::Vertex;

namespace
{
	struct Drawable
	{
		SDL_GPUBufferBinding binding{};
		Uint32 count{};
		glm::vec3 position{};
	};

	auto LoadGPUBuffer(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, std::vector<Vertex> x) -> Drawable
	{
		SDL_GPUBufferCreateInfo bufferInfo{};
		bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
		bufferInfo.size = static_cast<Uint32>(x.size()) * sizeof(decltype(x)::value_type);

		auto* buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

		SDL_GPUTransferBufferCreateInfo transferInfo{};
		transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		transferInfo.size = bufferInfo.size;
		auto* transfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

		// Load buffer data into the gpu.
		auto* map = SDL_MapGPUTransferBuffer(device, transfer, false);
		SDL_memcpy(map, x.data(), x.size() * sizeof(decltype(x)::value_type));
		SDL_UnmapGPUTransferBuffer(device, transfer);

		SDL_GPUTransferBufferLocation location{};
		location.transfer_buffer = transfer;

		SDL_GPUBufferRegion region{};
		region.buffer = buffer;
		region.size = bufferInfo.size;
		SDL_UploadToGPUBuffer(copyPass, &location, &region, false);
		SDL_ReleaseGPUTransferBuffer(device, transfer);

		return Drawable{.binding = {.buffer = buffer, .offset = 0}, .count = static_cast<Uint32>(x.size())};
	}

	auto Draw(SDL_Window* window, SDL_GPUDevice* device, SDL_GPUGraphicsPipeline* pipeline, Uint32 width, Uint32 height,
			  const std::vector<Drawable>& drawables) -> bool
	{
		Uint32 w{};
		Uint32 h{};

		// NOLINTNEXTLINE
		SDL_GetWindowSizeInPixels(window, reinterpret_cast<int*>(&w), reinterpret_cast<int*>(&h));

		SDL_GPUCommandBuffer* command{};
		{
			command = SDL_AcquireGPUCommandBuffer(device);
		}

		SDL_GPUTexture* swapchain{};

		{
			auto success = SDL_WaitAndAcquireGPUSwapchainTexture(command, window, &swapchain, &w, &h);

			if(!success)
			{
				return success;
			}
		}

		if(swapchain == nullptr)
		{
			SDL_SubmitGPUCommandBuffer(command);
			return true;
		}

		SDL_GPUColorTargetInfo color{};
		color.clear_color = SDL_FColor{.r = Color::Druid.redF(), .g = Color::Druid.greenF(), .b = Color::Druid.blueF(), .a = Color::Druid.alphaF()};
		color.load_op = SDL_GPU_LOADOP_CLEAR;
		color.store_op = SDL_GPU_STOREOP_STORE;
		color.texture = swapchain;

		auto* renderPass = SDL_BeginGPURenderPass(command, &color, 1, nullptr);

		{
			SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
		}

		const auto projection = glm::ortho(0.0F, static_cast<float>(width), static_cast<float>(height), 0.0F, 0.0F, 1.0F);
		const auto view = glm::mat4{1.0F};

		for(const auto& drawable : drawables)
		{
			auto model = glm::mat4{1.0F};
			model = glm::translate(model, drawable.position);
			model = glm::rotate(model, glm::radians(0.0F), {0.0F, 0.0F, 1.0F});

			const auto mvp = projection * view * model;
			{
				SDL_PushGPUVertexUniformData(command, 0, &mvp, sizeof(mvp));
			}

			{
				SDL_BindGPUVertexBuffers(renderPass, 0, &drawable.binding, 1);
			}
			SDL_DrawGPUPrimitives(renderPass, drawable.count, 2, 0, 0);
		}

		SDL_EndGPURenderPass(renderPass);
		SDL_SubmitGPUCommandBuffer(command);

		return true;
	}
}

auto main() -> int
try
{
	druid::graphics::Window window{};

	window.onEvent(
		[&window](const druid::graphics::EventWindow& e)
		{
			switch(e.type)
			{
				case druid::graphics::EventWindow::Type::Close_requested:
					window.close();
					break;
				default:
					break;
			}
		});

	auto* device = window.getRenderer()->handle();
	auto* pipeline = window.getDefaultPipeline()->handle();

	auto* command = SDL_AcquireGPUCommandBuffer(device);
	auto* copyPass = SDL_BeginGPUCopyPass(command);

	std::vector<Drawable> drawables;
	std::vector<Vertex> vertices;

	// NOLINTBEGIN
	vertices.emplace_back(Vertex{.position = {.x = -100.0F, .y = 100.0F}, .color = Color::Green.rgbaF()});
	vertices.emplace_back(Vertex{.position = {.x = 100.0F, .y = 100.0F}, .color = Color::White.rgbaF()});
	vertices.emplace_back(Vertex{.position = {.x = 0.0F, .y = 0.0F}, .color = Color::Blue.rgbaF()});
	drawables.emplace_back(LoadGPUBuffer(device, copyPass, std::move(vertices)));
	drawables.back().position = glm::vec3{300, 50, 0};

	vertices.emplace_back(Vertex{.position = {.x = 0.0F, .y = 0.0F}, .color = Color::White.rgbaF()});
	vertices.emplace_back(Vertex{.position = {.x = 0.0F, .y = 25.0F}, .color = Color::White.rgbaF()});
	vertices.emplace_back(Vertex{.position = {.x = (float)window.width(), .y = 0.0F}, .color = Color::White.rgbaF()});

	vertices.emplace_back(Vertex{.position = {.x = (float)window.width(), .y = 0.0F}, .color = Color::White.rgbaF()});
	vertices.emplace_back(Vertex{.position = {.x = 0.0F, .y = 25.0F}, .color = Color::White.rgbaF()});
	vertices.emplace_back(Vertex{.position = {.x = (float)window.width(), .y = 25.0F}, .color = Color::White.rgbaF()});
	// NOLINTEND

	drawables.emplace_back(LoadGPUBuffer(device, copyPass, std::move(vertices)));

	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(command);

	while(window.open())
	{
		window.processEvents();

		// window.beginFrame();
		// draw queues all new drawables not being tracked?
		// perform copy pass on queued drawables
		// unqueue drawables
		// perform render pass on bound drawables.
		// or!
		// use one buffer object
		// add drawable vertices to buffer object
		Draw(window.handle(), device, pipeline, window.width(), window.height(), drawables);
		// window.endFrame();
	}

	return 0;
}
catch(...)
{
	return EXIT_FAILURE;
}