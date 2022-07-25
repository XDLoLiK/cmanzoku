#ifndef STDFUNC_H
#define STDFUNC_H

#if   defined __FUNCTION__
	#define __FUNC__ __FUNCTION__
#elif defined __func__
	#define __FUNC__ __func__
#elif defined __PRETTY_FUNCTION__
	#define __FUNC__ __PRETTY_FUNCTION__
#else
	#define __FUNC__ "N/A"
#endif

#endif
