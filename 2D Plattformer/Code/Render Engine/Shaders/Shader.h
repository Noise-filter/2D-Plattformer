#ifndef SHADER_H
#define SHADER_H

#include "..\\Core\Directx11Includes.h"
#include <string>

template <class T>
class Shader
{
public:
	Shader();
	~Shader();

	bool Load(ID3D11Device* device, std::wstring filename);

	void SetShader(ID3D11DeviceContext* deviceContext);

	void* GetShaderBinaryData();
	SIZE_T GetShaderBinarySize();

private:
	T shader;
	ID3DBlob* shaderData;

};

template <class T>
Shader<T>::Shader()
{
	shader = NULL;
}

template <class T>
Shader<T>::~Shader()
{
	shader = NULL;
}

template <class T>
bool Shader<T>::Load(ID3D11Device* device, std::wstring filename)
{
	if (FAILED(D3DReadFileToBlob(filename.c_str(), &shaderData)))
	{
		return false;
	}

	if (typeid(T) == typeid(ID3D11PixelShader*))
	{
		if (FAILED(device->CreatePixelShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), NULL, (ID3D11PixelShader**)&shader)))
			return false;
	}
	else if (typeid(T) == typeid(ID3D11VertexShader*))
	{
		if (FAILED(device->CreateVertexShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), NULL, (ID3D11VertexShader**)&shader)))
			return false;
	}
	else if (typeid(T) == typeid(ID3D11HullShader*))
	{
		if (FAILED(device->CreateHullShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), NULL, (ID3D11HullShader**)&shader)))
			return false;
	}
	else if (typeid(T) == typeid(ID3D11ComputeShader*))
	{
		if (FAILED(device->CreateComputeShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), NULL, (ID3D11ComputeShader**)&shader)))
			return false;
	}
	else if (typeid(T) == typeid(ID3D11DomainShader*))
	{
		if (FAILED(device->CreateDomainShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), NULL, (ID3D11DomainShader**)&shader)))
			return false;
	}
	else if (typeid(T) == typeid(ID3D11GeometryShader*))
	{
		if (FAILED(device->CreateGeometryShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), NULL, (ID3D11GeometryShader**)&shader)))
			return false;
	}
	else
		return false;

	return true;
}

template <class T>
void Shader<T>::SetShader(ID3D11DeviceContext* deviceContext)
{
	if (typeid(T) == typeid(ID3D11PixelShader*))
		deviceContext->PSSetShader((ID3D11PixelShader*)shader, NULL, 0);
	else if (typeid(T) == typeid(ID3D11VertexShader*))
		deviceContext->VSSetShader((ID3D11VertexShader*)shader, NULL, 0);
	else if (typeid(T) == typeid(ID3D11HullShader*))
		deviceContext->HSSetShader((ID3D11HullShader*)shader, NULL, 0);
	else if (typeid(T) == typeid(ID3D11ComputeShader*))
		deviceContext->CSSetShader((ID3D11ComputeShader*)shader, NULL, 0);
	else if (typeid(T) == typeid(ID3D11DomainShader*))
		deviceContext->DSSetShader((ID3D11DomainShader*)shader, NULL, 0);
	else if (typeid(T) == typeid(ID3D11GeometryShader*))
		deviceContext->GSSetShader((ID3D11GeometryShader*)shader, NULL, 0);
}

template <class T>
void* Shader<T>::GetShaderBinaryData()
{
	return shaderData->GetBufferPointer();
}

template <class T>
SIZE_T Shader<T>::GetShaderBinarySize()
{
	return shaderData->GetBufferSize();
}

#endif