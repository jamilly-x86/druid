module;

#include <SDL3/SDL_gpu.h>
#include <array>
#include <filesystem>

export module druid.graphics.pipeline;

import druid.core.file;
import druid.graphics.vertex;

namespace druid::graphics
{
	export class Pipeline
	{
	public:
		struct Description
		{
			std::filesystem::path vertex;
			std::filesystem::path fragment;
			std::string entryPoint;
			SDL_GPUShaderFormat format{};
			SDL_GPUColorTargetDescription desc{};
			SDL_GPUDevice* device{};
		};

		Pipeline(const Description& x)
		{
			if(x.device == nullptr)
			{
				return;
			}

			this->device = x.device;
			this->format = x.format;
			this->entryPoint = x.entryPoint;

			auto* shaderVert = this->loadShader(x.vertex, true);
			auto* shaderFrag = this->loadShader(x.fragment, false);

			SDL_GPUGraphicsPipelineCreateInfo pipelineDesc{};
			pipelineDesc.target_info.num_color_targets = 1;
			pipelineDesc.target_info.color_target_descriptions = &x.desc;
			pipelineDesc.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
			pipelineDesc.vertex_shader = shaderVert;
			pipelineDesc.fragment_shader = shaderFrag;

			SDL_GPUVertexBufferDescription vertexDesc{};
			vertexDesc.slot = 0;
			vertexDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
			vertexDesc.instance_step_rate = 0;
			vertexDesc.pitch = sizeof(Vertex);

			std::array<SDL_GPUVertexAttribute, 3> vertexAttrib{};
			vertexAttrib[0].buffer_slot = 0;
			vertexAttrib[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
			vertexAttrib[0].location = 0;
			vertexAttrib[0].offset = 0;

			vertexAttrib[1].buffer_slot = 0;
			vertexAttrib[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
			vertexAttrib[1].location = 1;
			vertexAttrib[1].offset = sizeof(Vertex::position);

			vertexAttrib[2].buffer_slot = 0;
			vertexAttrib[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
			vertexAttrib[2].location = 2;
			vertexAttrib[2].offset = sizeof(Vertex::position) + sizeof(Vertex::color);

			pipelineDesc.vertex_input_state.num_vertex_buffers = 1;
			pipelineDesc.vertex_input_state.vertex_buffer_descriptions = &vertexDesc;
			pipelineDesc.vertex_input_state.vertex_attributes = vertexAttrib.data();
			pipelineDesc.vertex_input_state.num_vertex_attributes = static_cast<Uint32>(vertexAttrib.size());
			pipelineDesc.props = 0;
			pipelineDesc.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

			this->pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineDesc);

			SDL_ReleaseGPUShader(this->device, shaderVert);
			SDL_ReleaseGPUShader(this->device, shaderFrag);
		}

		~Pipeline()
		{
			SDL_ReleaseGPUGraphicsPipeline(this->device, this->pipeline);
		}

		[[nodiscard]] auto handle() const -> SDL_GPUGraphicsPipeline*
		{
			return this->pipeline;
		}

	private:
		auto loadShader(const std::filesystem::path& x, bool vertex) -> SDL_GPUShader*
		{
			const auto spv = druid::core::ReadFile(x);

			if(std::empty(spv) == true)
			{
				return nullptr;
			}

			SDL_GPUShaderCreateInfo info{};
			info.code = reinterpret_cast<const Uint8*>(spv.data());
			info.code_size = std::size(spv);
			info.entrypoint = this->entryPoint.c_str();
			info.format = this->format;
			info.num_uniform_buffers = vertex ? 1 : 0;
			info.stage = vertex ? SDL_GPU_SHADERSTAGE_VERTEX : SDL_GPU_SHADERSTAGE_FRAGMENT;
			return SDL_CreateGPUShader(this->device, &info);
		}

		std::string entryPoint;
		SDL_GPUShaderFormat format{};
		SDL_GPUDevice* device{};
		SDL_GPUGraphicsPipeline* pipeline{};
	};
}