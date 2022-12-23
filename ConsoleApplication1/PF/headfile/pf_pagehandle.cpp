#include "pf_pagehandle.h"
#include"pf_filehandle.h"
#include"pf_buffer.h"

extern PfBuffer pfBuffer;
PfPageHandle::~PfPageHandle() {
	if (dirty_)pfBuffer.writeBackPage(pffile_->fd_, num);
	pfBuffer.countDec(pffile_->fd_, num);
}