#ifndef COMMUN_H
#define COMMUN_H

template <class T>
	class StringBuffer {
	public:
		StringBuffer() : _str(0), _allocLen(0) { }
		~StringBuffer() { if(_allocLen) delete [] _str; }

		void sizeTo(size_t size) {
			if(_allocLen < size)
			{
				if(_allocLen) delete[] _str;
				_allocLen = max(size, initSize);
				_str = new T[_allocLen];
			}
		}
		void empty() {
			static T nullStr = 0; // routines may return an empty string, with null terminator, without allocating memory; a pointer to this null character will be returned in that case
			if(_allocLen == 0)
				_str = &nullStr;
			else
				_str[0] = 0;
		}

		operator T*() { return _str; }

	protected:
		static const int initSize = 1024;
		size_t _allocLen;
		T* _str;
	};

const wchar_t * char2wchar(const char *mbStr, UINT codepage, int lenIn=-1, int *pLenOut=NULL, int *pBytesNotProcessed=NULL);

void CStringToChar(CString str, char *&pstr);

#endif
