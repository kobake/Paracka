#include "Record.h"
#include "IoLib.h"

mystring Record::getFileName() const
{
	return CFilePath(this->filepath).GetTitle();
}
