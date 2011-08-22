/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file ListView.cpp
 * @author Emma Tresanszki
 *
 * Class for list views.
 */

#include "ListView.h"

namespace MoSync
{
	namespace UI
	{

	ListView::ListView() :
		Widget(MAW_LIST_VIEW)
	{
		// Set some common default values to
		// simplify when creating list views.
		this->fillSpaceHorizontally();
		this->wrapContentVertically();
	}

	void ListView::remove(Widget *widget)
	{
		mChildren.remove(&widget);
	}

	/**
	 * Destructor.
	 */
	ListView::~ListView()
	{
	}

	} // namespace UI
} // namespace MoSync
