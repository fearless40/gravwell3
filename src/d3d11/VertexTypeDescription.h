#pragma once

struct ID3D11InputLayout;

// Not reference counted as it can only be owend by the VertexTypeManager

struct VertexTypeDescription
{
public:
	VertexTypeDescription(void) : layout(nullptr), stride(0) {}
	~VertexTypeDescription(void) { }

	ID3D11InputLayout * layout;
	unsigned int		stride;

	bool operator == ( const VertexTypeDescription & vt )
	{
		return (layout == vt.layout ? true : false);
	}

	bool operator != ( const VertexTypeDescription & vt )
	{
		return (layout != vt.layout ? true : false);
	}
};

