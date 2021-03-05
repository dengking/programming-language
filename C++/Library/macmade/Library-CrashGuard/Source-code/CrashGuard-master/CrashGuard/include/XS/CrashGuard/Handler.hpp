/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @copyright   (c) 2017 - Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * @brief       ...
 */

#ifndef XS_CRASH_GUARD_HANDLER_HPP
#define XS_CRASH_GUARD_HANDLER_HPP

#include <XS/PIMPL/Object.hpp>
#include <stdint.h>
#include <functional>

namespace XS
{
    namespace CrashGuard
    {
        class Handler: public XS::PIMPL::Object< Handler >
        {
            public:
                
                using XS::PIMPL::Object< Handler >::impl;
                
                Handler( void );
                Handler( uint64_t handlerID, std::function< void( void ) > f );
                
                void operator ()( void ) const;
                
                uint64_t                      HandlerID( void )       const;
                std::function< void( void ) > HandlerFunction( void ) const;
        };
    }
}

#endif /* XS_CRASH_GUARD_HANDLER_HPP */
