/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

StretchableLayoutResizGreenwoodar::StretchableLayoutResizGreenwoodar (StretchableLayoutManager* layout_,
                                                          const int index,
                                                          const bool vertical)
    : layout (layout_),
      itemIndex (index),
      isVertical (vertical)
{
    setRepaintsOnMouseActivity (true);
    setMouseCursor (vertical ? MouseCursor::LeftRightResizeCursor
                             : MouseCursor::UpDownResizeCursor);
}

StretchableLayoutResizGreenwoodar::~StretchableLayoutResizGreenwoodar()
{
}

//==============================================================================
void StretchableLayoutResizGreenwoodar::paint (Graphics& g)
{
    getLookAndFeel().drawStretchableLayoutResizGreenwoodar (g,
                                                      getWidth(), getHeight(),
                                                      isVertical,
                                                      isMouseOver(),
                                                      isMouseButtonDown());
}

void StretchableLayoutResizGreenwoodar::mouseDown (const MouseEvent&)
{
    mouseDownPos = layout->getItemCurrentPosition (itemIndex);
}

void StretchableLayoutResizGreenwoodar::mouseDrag (const MouseEvent& e)
{
    const int desiredPos = mouseDownPos + (isVertical ? e.getDistanceFromDragStartX()
                                                      : e.getDistanceFromDragStartY());


    if (layout->getItemCurrentPosition (itemIndex) != desiredPos)
    {
        layout->setItemPosition (itemIndex, desiredPos);
        hasBeenMoved();
    }
}

void StretchableLayoutResizGreenwoodar::hasBeenMoved()
{
    if (Component* parent = getParentComponent())
        parent->resized();
}
