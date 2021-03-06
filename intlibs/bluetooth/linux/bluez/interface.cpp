/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * File:   interface.cpp
 * Author: Ali Mosavian
 *
 * Created on July 29, 2009
 */
#include <cstdlib>
#include <bluetooth/bluetooth.h>

#include "../../config_bluetooth.h"
#include "../../btinit.h"
#include "../../discovery.h"
#include "bluetoothbluez.hpp"
#include "helpers/helpers.h"





using namespace Bluetooth;

/**
 * Globals
 */
static BlueZ::BluetoothBluez* gInstance = NULL;


namespace Bluetooth
{
    BluetoothStack gBluetoothStack = BTSTACK_BLUEZ;


/**
 * Will initialize the bluetooth system
 *  
 */
void MABtInit ( void )
{
    MAASSERT( gInstance == NULL );
    gInstance = new BlueZ::BluetoothBluez( );
}

/**
 * Will close the bluetooth system
 *
 */
void MABtClose ( void )
{
    if ( gInstance == NULL )
    {
        LOGBT( "gInstance is NULL" );
        return;
    }

    delete gInstance;
}


/**
 * Returns the adress of the bluetooth interface
 *
 * @param a     Structure to fill with interface address
 *
 * @return < 0    On failure.
 */
int getLocalAddress ( MABtAddr& a )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getLocalAddress( a );
}


/**
 * Returns the device discovery state.
 *
 * @return	0 - Still working
 *          1 - Finished successfully
 *        < 0 - (CONNERR) Failed.
 */
int maBtDiscoveryState ( void )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getState( );
}


/**
 * Cancels an on going device discovery.
 * Note: If an operation was canceled, its last BT event will have 
 *       the status CONNERR_CANCELED. This is an asynchronous operation. 
 *       It is not safe to start another discovery before you've recieved 
 *       the CONNERR_CANCELED event.
 *
 * @return 0 if there was no active operation
 *         1 if there was.
 */
int maBtCancelDiscovery ( void )
{
    MAASSERT( gInstance != NULL );
    return gInstance->cancelDeviceDiscovery( );
}


/**
 * Start a new device discovery operation in the background
 * and invokes the callback once it has finished.
 *
 * Note: Only one discovery operation can be active at
 * 	 a time.
 *
 * @param cb	- The callback which will be invoked once the
 *                discovery operation has finished.
 * @param n 	- Attempt to discover device names
 */
int maBtStartDeviceDiscovery ( MABtCallback cb,
                               bool n )
{
    MAASSERT( gInstance != NULL );
    return gInstance->startDiscovery( cb, n );
}


/**
 * Returns the next discovered device.
 *
 * @param d Pointer to the structure to fill.
 *
 * @return 1 if there was a device, 0 if not
 */
int maBtGetNewDevice ( MABtDeviceNative* d )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getNextDevice( d );
}


/**
 * Starts a new service discovery operation given a device
 * adress and a family of services to search for.
 *
 * @param a    The address of the device to do service discovery on.
 * @param u    The service family UUID to search for.
 * @param cb   The callback to invoke once discovery has finished.
 */
int maBtStartServiceDiscovery ( const MABtAddr* a,
                                const MAUUID* u,
                                MABtCallback cb )
{
    MAASSERT( gInstance != NULL );
    return gInstance->startServiceDiscovery(cb, a, u );
}

/**
 * Returns the next service, can be called once a service
 * discovery has finished.
 *
 * @param d   Pointer to structure to fill
 *
 * @return > 0 If there was any more services to get
 */
int maBtGetNewService ( MABtServiceNative* d )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getNextService( d );
}


/**
 * Returns the size of the next service, of any.
 *
 * @param d     Pointer to the structure to fill.
 *
 * @return > 0 if there was a service, 0 if not
 */
int maBtGetNextServiceSize ( MABtServiceSize* d )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getNextServiceSize( d );
}

}
