#pragma once

#include <afxstr.h>

#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif



namespace JD
{
	template< typename CharT>
	class MFC_UTILITIES_API  Buffer
	{
		CString m_bufferImpl;
		CharT* m_pointerToBuffer;
	public:
		Buffer(size_t num_bytes)
		{
			m_pointerToBuffer = m_bufferImpl.GetBuffer(num_bytes);
		}
		~Buffer()
		{
			m_bufferImpl.ReleaseBuffer();
		}
		operator CharT*() const noexcept
		{
			return m_pointerToBuffer;
		}
	};
}
