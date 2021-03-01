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

#include <XS/CrashGuard.hpp>
#include <XS/CrashGuard/Handler.hpp>
#include <mutex>
#include <vector>

static std::once_flag                           once;
static std::recursive_mutex                   * rmtx;
static std::vector< XS::CrashGuard::Handler > * handlers;

static void init( void );

namespace XS
{
    namespace CrashGuard
    {
        uint64_t InstallHandler( std::function< void( void ) > handler )
        {
            std::call_once( once, init );
            
            {
                std::lock_guard< std::recursive_mutex > l( *( rmtx ) );
                Handler                                 h( handlers->size(), handler );
                
                handlers->push_back( h );
                
                return h.HandlerID();
            }
        }
        
        void RemoveHandler( uint64_t handlerID )
        {
            std::call_once( once, init );
            
            {
                std::lock_guard< std::recursive_mutex > l( *( rmtx ) );
                
                handlers->erase
                (
                    std::remove_if
                    (
                        handlers->begin(),
                        handlers->end(),
                        [ = ]( const Handler & h )
                        {
                            return h.HandlerID() == handlerID;
                        }
                    ),
                    handlers->end()
                );
            }
        }
    }
}

#ifdef _WIN32

static void init( void )
{
    rmtx     = new std::recursive_mutex();
    handlers = new std::vector< XS::CrashGuard::Handler >();
}

#elif defined( __APPLE__ )

#include <signal.h>

static void sig( int signo );

static struct sigaction sigaction_sigabrt      = {};
static struct sigaction sigaction_sigsegv      = {};
static struct sigaction sigaction_sigbus       = {};
static struct sigaction sigaction_sigill       = {};
static struct sigaction sigaction_sigfpe       = {};
static struct sigaction sigaction_sigpipe      = {};
static struct sigaction sigaction_prev_sigabrt = {};
static struct sigaction sigaction_prev_sigsegv = {};
static struct sigaction sigaction_prev_sigbus  = {};
static struct sigaction sigaction_prev_sigill  = {};
static struct sigaction sigaction_prev_sigfpe  = {};
static struct sigaction sigaction_prev_sigpipe = {};

static void init( void )
{
    rmtx     = new std::recursive_mutex();
    handlers = new std::vector< XS::CrashGuard::Handler >();
    
    memset( &sigaction_sigabrt, 0, sizeof( struct sigaction ) );
    memset( &sigaction_sigsegv, 0, sizeof( struct sigaction ) );
    memset( &sigaction_sigbus,  0, sizeof( struct sigaction ) );
    memset( &sigaction_sigill,  0, sizeof( struct sigaction ) );
    memset( &sigaction_sigfpe,  0, sizeof( struct sigaction ) );
    memset( &sigaction_sigpipe, 0, sizeof( struct sigaction ) );
    
    memset( &sigaction_prev_sigabrt, 0, sizeof( struct sigaction ) );
    memset( &sigaction_prev_sigsegv, 0, sizeof( struct sigaction ) );
    memset( &sigaction_prev_sigbus,  0, sizeof( struct sigaction ) );
    memset( &sigaction_prev_sigill,  0, sizeof( struct sigaction ) );
    memset( &sigaction_prev_sigfpe,  0, sizeof( struct sigaction ) );
    memset( &sigaction_prev_sigpipe, 0, sizeof( struct sigaction ) );
    
    sigaction_sigabrt.sa_handler = sig;
    sigaction_sigsegv.sa_handler = sig;
    sigaction_sigbus.sa_handler  = sig;
    sigaction_sigill.sa_handler  = sig;
    sigaction_sigfpe.sa_handler  = sig;
    sigaction_sigpipe.sa_handler = sig;
    
    sigaction( SIGABRT, &sigaction_sigabrt, &sigaction_prev_sigabrt );
    sigaction( SIGSEGV, &sigaction_sigsegv, &sigaction_prev_sigsegv );
    sigaction( SIGBUS,  &sigaction_sigbus,  &sigaction_prev_sigbus );
    sigaction( SIGILL,  &sigaction_sigill,  &sigaction_prev_sigill );
    sigaction( SIGFPE,  &sigaction_sigfpe,  &sigaction_prev_sigfpe );
    sigaction( SIGPIPE, &sigaction_sigpipe, &sigaction_prev_sigpipe );
}

static void sig( int signo )
{
    for( const auto & h: *( handlers ) )
    {
        h();
    }
    
    handlers->clear();
    
    switch( signo )
    {
        case SIGABRT: sigaction( SIGABRT, &sigaction_prev_sigabrt, nullptr ); break;
        case SIGSEGV: sigaction( SIGSEGV, &sigaction_prev_sigsegv, nullptr ); break;
        case SIGBUS:  sigaction( SIGBUS,  &sigaction_prev_sigbus,  nullptr ); break;
        case SIGILL:  sigaction( SIGILL,  &sigaction_prev_sigill,  nullptr ); break;
        case SIGFPE:  sigaction( SIGFPE,  &sigaction_prev_sigfpe,  nullptr ); break;
        case SIGPIPE: sigaction( SIGPIPE, &sigaction_prev_sigpipe, nullptr ); break;
        default:                                                              break;
    }
}

#endif
