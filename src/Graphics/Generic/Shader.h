#pragma once

namespace Generic::graphics {
	
	template <typename VertexShader, typename PixelShader>
	struct ShaderResource {
		VertexShader & vxs;
		PixelShader & pxs;
	};
}