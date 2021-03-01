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

#include <XS/CrashGuard/Handler.hpp>

namespace XS
{
    template<>
    class XS::PIMPL::Object< CrashGuard::Handler >::IMPL
    {
        public:
            
            IMPL( void );
            IMPL( uint64_t handlerID, std::function< void( void ) > f );
            IMPL( const IMPL & o );
            ~IMPL( void );
            
            uint64_t                      _handlerID;
            std::function< void( void ) > _handlerFunction;
    };
}

#define XS_PIMPL_CLASS XS::CrashGuard::Handler
#include <XS/PIMPL/Object-IMPL.hpp>

namespace XS
{
    namespace CrashGuard
    {
        Handler::Handler( void ):
            XS::PIMPL::Object< Handler >()
        {}
        
        Handler::Handler( uint64_t handlerID, std::function< void( void ) > f ):
            XS::PIMPL::Object< Handler >( handlerID, f )
        {}
        
        void Handler::operator ()( void ) const
        {
            if( this->impl->_handlerFunction != nullptr )
            {
                this->impl->_handlerFunction();
            }
        }
        
        uint64_t Handler::HandlerID( void ) const
        {
            return this->impl->_handlerID;
        }
        
        std::function< void( void ) > Handler::HandlerFunction( void ) const
        {
            return this->impl->_handlerFunction;
        }
    }
    
    XS::PIMPL::Object< CrashGuard::Handler >::IMPL::IMPL( void ):
        _handlerID( 0 ),
        _handlerFunction( nullptr )
    {}
    
    XS::PIMPL::Object< CrashGuard::Handler >::IMPL::IMPL( uint64_t handlerID, std::function< void( void ) > f ):
        _handlerID( handlerID ),
        _handlerFunction( f )
    {}
    
    XS::PIMPL::Object< CrashGuard::Handler >::IMPL::IMPL( const IMPL & o ):
        _handlerID( o._handlerID ),
        _handlerFunction( o._handlerFunction )
    {}
    
    XS::PIMPL::Object< CrashGuard::Handler >::IMPL::~IMPL( void )
    {}
}
