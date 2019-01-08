#pragma once

namespace Generic::graphics {
	
	template <typename VertexShader, typename PixelShader>
	struct ShaderResource {
		VertexShader  vs;
		PixelShader   ps;
	};
}