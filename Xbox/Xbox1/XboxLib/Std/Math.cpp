/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#include "XboxLib.hpp"
#include "Std/Math.hpp"

namespace XboxLib
{
	namespace Math
	{
		_RealConstants RealConstants = {
			0.0f,
			1.0f,
			90.f,
			M_PI,
			0.01745f,	// degrees in one radian
			0.05236f,	// 3 degrees in radians
			1.5707964f,	// 90 degrees in radians
			6.2831855f,	// 360 degrees in radians
			10.0f,
		};

		API_FUNC_NAKED double __stdcall acos(double x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		qword ptr [ebp+8]		// Load real from stack
                fld		st(0)					// Load x
                fld		st(0)					// Load x
                fmul							// Multiply (x squared)
                fld1							// Load 1
                fsubr							// 1 - (x squared)
                fsqrt							// Square root of (1 - x squared)
                fxch							// Exchange st, st(1)
                fpatan							// This gives the arc cosine !
                pop		ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall acosf(float x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		dword ptr [ebp+8]		// Load real from stack
                fld		st(0)					// Load x
                fld		st(0)					// Load x
                fmul							// Multiply (x squared)
                fld1							// Load 1
                fsubr							// 1 - (x squared)
                fsqrt							// Square root of (1 - x squared)
                fxch							// Exchange st, st(1)
                fpatan							// This gives the arc cosine !
                pop		ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall asin(double x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		qword ptr [ebp+8]		// Load real from stack
                fld		st(0)					// Load x
                fld		st(0)					// Load x
                fmul							// Multiply (x squared)
                fld1							// Load 1
                fsubr							// 1 - (x squared)
                fsqrt							// Square root of (1 - x squared)
                fpatan							// This gives the arc sin !
                pop		ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall asinf(float x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		dword ptr [ebp+8]		// Load real from stack
                fld		st(0)					// Load x
                fld		st(0)					// Load x
                fmul							// Multiply (x squared)
                fld1							// Load 1
                fsubr							// 1 - (x squared)
                fsqrt							// Square root of (1 - x squared)
                fpatan							// This gives the arc sin !
                pop		ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall atan(double x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		qword ptr [ebp+8]		// Load real from stack
                fld1							// Load 1
                fpatan							// Take the arctangent
                pop		ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall atanf(float x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		dword ptr [ebp+8]		// Load real from stack
                fld1							// Load 1
                fpatan							// Take the arctangent
                pop		ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall atan2(double x, double y)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		qword ptr [ebp+8]		// Load real from stack
				fld		qword ptr [ebp+16]		// Load real from stack
                fpatan							// Take the arctangent
                pop		ebp
                ret		8 + 8
			}
		}

		API_FUNC_NAKED double __stdcall ceil(double x)
		{
			__asm {
				push	ebp
				mov		ebp, esp
				sub		esp, 4					// Allocate temporary space
				fld		qword ptr [ebp+8]		// Load real from stack
				fstcw	[ebp-2]					// Save control word
				fclex							// Clear exceptions
				mov		word ptr [ebp-4], 0xB63	// Rounding control word
				fldcw	[ebp-4]					// Set new rounding control
				frndint							// Round to integer
				fclex							// Clear exceptions
				fldcw	[ebp-2]					// Restore control word
				mov		esp, ebp
				pop		ebp
				ret		8
			}
		}

		API_FUNC_NAKED float __stdcall ceilf(float x)
		{
			__asm {
				push	ebp
				mov		ebp, esp
				sub		esp, 4					// Allocate temporary space
				fld		dword ptr [ebp+8]		// Load real from stack
				fstcw	[ebp-2]					// Save control word
				fclex							// Clear exceptions
				mov		word ptr [ebp-4], 0xB63	// Rounding control word
				fldcw	[ebp-4]					// Set new rounding control
				frndint							// Round to integer
				fclex							// Clear exceptions
				fldcw	[ebp-2]					// Restore control word
				mov		esp, ebp
				pop		ebp
				ret		4
			}
		}

		API_FUNC_NAKED double __stdcall cos(double x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		qword ptr [ebp+8]		// Load real from stack
                fcos							// Take the cosine
                pop		ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall cosf(float x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		dword ptr [ebp+8]		// Load real from stack
                fcos							// Take the cosine
                pop		ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall cosh(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,8                   // Allocate temporary space
                fld     qword ptr [ebp+8]       // Load real from stack
                fchs                            // Set x = -x
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                fstp    qword ptr [ebp-8]       // Save exp(-x)
                fld     qword ptr [ebp+8]       // Load real from stack
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Compute exp(-x)
                fstp    st(1)                   // Set new stack top and pop
                fld     qword ptr [ebp-8]       // Get exp(x)
                fadd                            // Compute exp(x) + exp(-x)
                fld1                            // Load the constant 1
                fld1                            // Load the constant 1
                fadd                            // Set divisor to 2
                fdiv                            // Compute the hyperbolic cosine
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall coshf(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,4                   // Allocate temporary space
                fld     dword ptr [ebp+8]       // Load real from stack
                fchs                            // Set x = -x
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                fstp    dword ptr [ebp-4]       // Save exp(-x)
                fld     dword ptr [ebp+8]       // Load real from stack
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Compute exp(-x)
                fstp    st(1)                   // Set new stack top and pop
                fld     dword ptr [ebp-4]       // Get exp(x)
                fadd                            // Compute exp(x) + exp(-x)
                fld1                            // Load the constant 1
                fld1                            // Load the constant 1
                fadd                            // Set divisor to 2
                fdiv                            // Compute the hyperbolic cosine
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall exp(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,8                   // Allocate temporary space
                fld     qword ptr [ebp+8]       // Load real from stack
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                fst     qword ptr [ebp-8]       // Throw away scale factor
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall expf(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,4                   // Allocate temporary space
                fld     dword ptr [ebp+8]       // Load real from stack
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                fst     dword ptr [ebp-4]       // Throw away scale factor
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall fabs(double x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		qword ptr [ebp+8]		// Load real from stack
                fabs							// Take the cosine
                pop		ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall fabsf(float x)
		{
			__asm {
				push	ebp
                mov		ebp,esp
                fld		dword ptr [ebp+8]		// Load real from stack
                fabs							// Take the cosine
                pop		ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall floor(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,4                   // Allocate temporary space
                fld     qword ptr [ebp+8]       // Load real from stack
                fstcw   [ebp-2]                 // Save control word
                fclex                           // Clear exceptions
                mov     word ptr [ebp-4],0763h  // Rounding control word
                fldcw   [ebp-4]                 // Set new rounding control
                frndint                         // Round to integer
                fclex                           // Clear exceptions
                fldcw   [ebp-2]                 // Restore control word
                mov     esp,ebp
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall floorf(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,4                   // Allocate temporary space
                fld     dword ptr [ebp+8]       // Load real from stack
                fstcw   [ebp-2]                 // Save control word
                fclex                           // Clear exceptions
                mov     word ptr [ebp-4],0763h  // Rounding control word
                fldcw   [ebp-4]                 // Set new rounding control
                frndint                         // Round to integer
                fclex                           // Clear exceptions
                fldcw   [ebp-2]                 // Restore control word
                mov     esp,ebp
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall fmod(double x, double y)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     qword ptr [ebp+16]      // Load real from stack
                fld     qword ptr [ebp+8]       // Load real from stack
__fmod1:        fprem                           // Get the partial remainder
                fstsw   ax                      // Get coprocessor status
                test    ax,0400h                // Complete remainder ?
                jnz     __fmod1                 // No, go get next remainder
                fstp    st(1)                   // Set new top of stack
                pop     ebp
                ret		8 + 8
			}
		}

		API_FUNC_NAKED double __stdcall frexp(double x, int* n)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                push    edi                     // Save register edi
                fld     qword ptr [ebp+8]       // Load real from stack
                mov     edi,dword ptr [ebp+16]  // Put exponent address in edi
                ftst                            // Test st for zero
                fstsw   ax                      // Put test result in ax
                sahf                            // Set flags based on test
                jnz     __frexp1                // Re-direct if not zero
                fld     st                      // Set exponent to zero
                jmp     __frexp2                // End of case
__frexp1:       fxtract                         // Get exponent and significand
                fld1                            // Load constant 1
                fld1                            // Load constant 1
                fadd                            // Constant 2
                fdiv                            // Significand / 2
                fxch                            // Swap st, st(1)
                fld1                            // Load constant 1
                fadd                            // Increment exponent
                fistp   dword ptr [edi]         // Store result exponent and pop
__frexp2:       pop     edi                     // Restore register edi
                mov     esp,ebp
                pop     ebp
                ret		8 + 4
			}
		}

		API_FUNC_NAKED long __stdcall ftol(double x)
		{
			__asm {
				fnstcw  word ptr [esp-2]
                mov     ax, word ptr [esp-2]
                or      ax, 0C00h
                mov     word ptr [esp-4], ax
                fldcw   word ptr [esp-4]
                fistp   qword ptr [esp-12]
                fldcw   word ptr [esp-2]
                mov     eax, dword ptr [esp-12]
                mov     edx, dword ptr [esp-8]
                ret		8
			}
		}

		API_FUNC_NAKED double __stdcall ldexp(double x, int n)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,8                   // Allocate temporary space
                fild    dword ptr [ebp+16]      // Load n as integer
                fld     qword ptr [ebp+8]       // Load real from stack
                fscale                          // Compute 2 to the n
                fstp    st(1)                   // Set new top of stack
                fst     qword ptr [ebp-8]       // Store result
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		8 + 4
			}
		}

		API_FUNC_NAKED double __stdcall log(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     qword ptr [ebp+8]       // Load real from stack
                fldln2                          // Load log base e of 2
                fxch    st(1)                   // Exchange st, st(1)
                fyl2x                           // Compute the natural log(x)
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall logf(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     dword ptr [ebp+8]       // Load real from stack
                fldln2                          // Load log base e of 2
                fxch    st(1)                   // Exchange st, st(1)
                fyl2x                           // Compute the natural log(x)
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall log10(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     qword ptr [ebp+8]       // Load real from stack
                fldlg2                          // Load log base 10 of 2
                fxch    st(1)                   // Exchange st, st(1)
                fyl2x                           // Compute the log base 10(x)
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall log10f(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     dword ptr [ebp+8]       // Load real from stack
                fldlg2                          // Load log base 10 of 2
                fxch    st(1)                   // Exchange st, st(1)
                fyl2x                           // Compute the log base 10(x)
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall modf(double x, double* y)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                push    edi                     // Save register edi
                fld     qword ptr [ebp+8]       // Load real from stack
                mov     edi,dword ptr [ebp+16]  // Put integer address in edi
                fld     st                      // Duplicate st
                frndint                         // Round to integer
                fcom    st(1)                   // Compare with orignal value
                fstsw   ax                      // Get the FPU status word
                test    byte ptr [ebp+15],080h  // Test if number is negative
                jz      __fmodf1                // Re-direct if positive
                sahf                            // Store AH to flags
                jnb     __fmodf2                // Re-direct if greater or equal
                fld1                            // Load the constant 1
                fadd                            // Increment integer part
                jmp     __fmodf2                // End of case
__fmodf1:       sahf                            // Store AH to flags
                jna     __fmodf2                // Re-direct if less or equal
                fld1                            // Load constant 1
                fsub                            // Decrement integer part
__fmodf2:       fst     qword ptr [edi]         // Store integer part
                fsub                            // Subtract to get fraction
                pop     edi                     // Restore register edi
                mov     esp,ebp                 // Restore stack pointer
                pop     ebp
                ret		8 + 4
			}
		}

		API_FUNC_NAKED double __stdcall pow(double x, double y)
		{
			static float fzero = 0;
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,12                  // Allocate temporary space
                push    edi                     // Save register edi
                push    eax                     // Save register eax
                mov     dword ptr [ebp-12],0    // Set negation flag to zero
                fld     qword ptr [ebp+16]      // Load real from stack
                fld     qword ptr [ebp+8]       // Load real from stack
                mov     edi, offset fzero		// Point to real zero
                fcom    qword ptr [edi]         // Compare x with zero
                fstsw   ax                      // Get the FPU status word
                mov     al,ah                   // Move condition flags to AL
                lahf                            // Load Flags into AH
                and     al,    01000101B        // Isolate  C0, C2 and C3
                and     ah,not 01000101B        // Turn off CF, PF and ZF
                or      ah,al                   // Set new  CF, PF and ZF
                sahf                            // Store AH into Flags
                jb      __fpow1                 // Re-direct if x < 0
                ja      __fpow3                 // Re-direct if x > 0
                fxch                            // Swap st, st(1)
                fcom    qword ptr [edi]         // Compare y with zero
                fxch                            // Restore x as top of stack
                fstsw   ax                      // Get the FPU status word
                mov     al,ah                   // Move condition flags to AL
                lahf                            // Load Flags into AH
                and     al,    01000101B        // Isolate  C0, C2 and C3
                and     ah,not 01000101B        // Turn off CF, PF and ZF
                or      ah,al                   // Set new  CF, PF and ZF
                sahf                            // Store AH into Flags
                ja      __fpow3                 // Re-direct if y > 0
                fstp    st(1)                   // Set new stack top and pop
                mov     eax,1                   // Set domain error (EDOM)
                jmp     __fpow5                 // End of case
__fpow1:        fxch                            // Put y on top of stack
                fld    st                       // Duplicate y as st(1)
                frndint                         // Round to integer
                fxch                            // Put y on top of stack
                fcomp                           // y = int(y) ?
                fstsw   ax                      // Get the FPU status word
                mov     al,ah                   // Move condition flags to AL
                lahf                            // Load Flags into AH
                and     al,    01000101B        // Isolate  C0, C2 and C3
                and     ah,not 01000101B        // Turn off CF, PF and ZF
                or      ah,al                   // Set new  CF, PF and ZF
                sahf                            // Store AH into Flags
                je      __fpow2                 // Proceed if y = int(y)
                fstp    st(1)                   // Set new stack top and pop
                fldz                            // Set result to zero
                fstp    st(1)                   // Set new stack top and pop
                mov     eax,1                   // Set domain error (EDOM)
                jmp     __fpow5                 // End of case
__fpow2:        fist    dword ptr [ebp-12]      // Store y as integer
                and     dword ptr [ebp-12],1    // Set bit if y is odd
                fxch                            // Put x on top of stack
                fabs                            // x = |x|
__fpow3:        fldln2                          // Load log base e of 2
                fxch    st(1)                   // Exchange st, st(1)
                fyl2x                           // Compute the natural log(x)
                fmul                            // Compute y * ln(x)
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                test    dword ptr [ebp-12],1    // Negation required ?
                jz      __fpow4                 // No, re-direct
                fchs                            // Negate the result
__fpow4:        fstp    qword ptr [ebp-8]       // Save (double)pow(x, y)
                fld     qword ptr [ebp-8]       // Load (double)pow(x, y)
                fxam                            // Examine st
                fstsw   ax                      // Get the FPU status word
                cmp     ah,5                    // Infinity ?
                jne     __fpow6                 // No, end of case
                mov     eax,2                   // Set range error (ERANGE)
                                                // Get errno pointer offset
__fpow5:        int     3
                mov     edi,0                   // TODO: offset flat:__crt_errno
                mov     edi,[edi]               // Get C errno variable pointer
                mov     dword ptr [edi],eax     // Set errno
__fpow6:        pop     eax                     // Restore register eax
                pop     edi                     // Restore register edi
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED double __stdcall sin(double x)
		{
			__asm {
				push    ebp                     // Save register bp
                mov     ebp,esp                 // Point to the stack frame
                fld     qword ptr [ebp+8]       // Load real from stack
                fsin                            // Take the sine
                pop     ebp                     // Restore register bp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall sinf(float x)
		{
			__asm {
				push    ebp                     // Save register bp
                mov     ebp,esp                 // Point to the stack frame
                fld     dword ptr [ebp+8]       // Load real from stack
                fsin                            // Take the sine
                pop     ebp                     // Restore register bp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall sinh(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,8                   // Allocate temporary space
                fld     qword ptr [ebp+8]       // Load real from stack
                fchs                            // Set x = -x
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                fstp    qword ptr [ebp-8]       // Save exp(-x)
                fld     qword ptr [ebp+8]       // Load real from stack
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Compute exp(-x)
                fstp    st(1)                   // Set new stack top and pop
                fld     qword ptr [ebp-8]       // Get exp(x)
                fsub                            // Compute exp(x) - exp(-x)
                fld1                            // Load the constant 1
                fld1                            // Load the constant 1
                fadd                            // Set divisor to 2
                fdiv                            // Compute the hyperbolic sine
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED double __stdcall sqrt(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     qword ptr [ebp+8]       // Load real from stack
                fsqrt                           // Take the square root
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall sqrtf(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     dword ptr [ebp+8]       // Load real from stack
                fsqrt                           // Take the square root
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall tan(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,4                   // Allocate temporary space
                fld     qword ptr [ebp+8]       // Load real from stack
                fptan                           // Take the tangent
                fstp    dword ptr [ebp-4]       // Throw away the constant 1
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		8
			}
		}

		API_FUNC_NAKED float __stdcall tanf(float x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                sub     esp,4                   // Allocate temporary space
                fld     dword ptr [ebp+8]       // Load real from stack
                fptan                           // Take the tangent
                fstp    dword ptr [ebp-4]       // Throw away the constant 1
                mov     esp,ebp                 // Deallocate temporary space
                pop     ebp
                ret		4
			}
		}

		API_FUNC_NAKED double __stdcall tanh(double x)
		{
			__asm {
				push    ebp
                mov     ebp,esp
                fld     qword ptr [ebp+8]       // Load real from stack
                fld     st                      // Duplicate stack top
                fadd                            // Compute 2 * x
                fldl2e                          // Load log base 2(e)
                fmulp   st(1),st                // Multiply x * log base 2(e)
                fst     st(1)                   // Push result
                frndint                         // Round to integer
                fsub    st(1),st                // Subtract
                fxch                            // Exchange st, st(1)
                f2xm1                           // Compute 2 to the (x - 1)
                fld1                            // Load real number 1
                fadd                            // 2 to the x
                fscale                          // Scale by power of 2
                fstp    st(1)                   // Set new stack top and pop
                fld1                            // Load constant 1
                fadd                            // Compute exp(2*x)+1
                fld1                            // Load the constant 1
                fld1                            // Load the constant 1
                fadd                            // Set divisor to 2
                fdivr                           // Compute 2/(exp(2*x)+1)
                fld1                            // Load constant 1
                fsubr                           // Compute the hyperbolic tangent
                pop     ebp                     // Restore register bp
                ret		8
			}
		}
	};
};