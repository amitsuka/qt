/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgesturerecognizer.h"

#include "private/qgesture_p.h"

QT_BEGIN_NAMESPACE

/*!
   \class QGestureRecognizer
   \since 4.6
   \brief The QGestureRecognizer class provides the infrastructure for gesture recognition.
   \ingroup gestures

   Gesture recognizers are responsible for creating and managing QGesture objects and
   monitoring input events sent to QWidget and QGraphicsObject subclasses.
   QGestureRecognizer is the base class for implementing custom gestures.

   Developers that only need to provide gesture recognition for standard gestures do not
   need to use this class directly. Instances will be created behind the scenes by the
   framework.

   \section1 Recognizing Gestures

   The process of recognizing gestures involves filtering input events sent to specific
   objects, and modifying the associated QGesture objects to include relevant information
   about the user's input.

   Gestures are created when the framework calls createGesture() to handle user input
   for a particular instance of a QWidget or QGraphicsObject subclass. A QGesture object
   is created for each widget or item that is configured to use gestures.

   Once a QGesture has been created for a target object, the gesture recognizer will
   receive events for it in its filterEvent() handler function.

   When a gesture is canceled, the reset() function is called, giving the recognizer the
   chance to update the appropriate properties in the corresponding QGesture object.

   \section1 Supporting New Gestures

   To add support for new gestures, you need to derive from QGestureRecognizer to create
   a custom recognizer class, construct an instance of this class, and register it with
   the application by calling QApplication::registerGestureRecognizer(). You can also
   subclass QGesture to create a custom gesture class, or rely on dynamic properties
   to express specific details of the gesture you want to handle.

   Your custom QGestureRecognizer subclass needs to reimplement the filterEvent() function
   to handle and filter the incoming input events for QWidget and QGraphicsObject subclasses.
   Although the logic for gesture recognition is implemented in this function, you can
   store persistent information about the state of the recognition process in the QGesture
   object supplied. The filterEvent() function must return a value of Qt::GestureState that
   indicates the state of recognition for a given gesture and target object. This determines
   whether or not a gesture event will be delivered to a target object.

   If you choose to represent a gesture by a custom QGesture subclass, you will need to
   reimplement the createGesture() function to construct instances of your gesture class.
   Similarly, you may need to reimplement the reset() function if your custom gesture
   objects need to be specially handled when a gesture is canceled.

   \sa QGesture
*/

/*!
    \enum QGestureRecognizer::ResultFlags

    This enum describes the result of the current event filtering step in
    a gesture recognizer state machine.

    The result consists of a state value (one of Ignore, NotGesture,
    MaybeGesture, GestureTriggered, GestureFinished) and an optional hint
    (ConsumeEventHint).

    \value Ignore The event does not change the state of the recognizer.

    \value NotGesture The event made it clear that it is not a gesture. If the
    gesture recognizer was in GestureTriggered state before, then the gesture
    is canceled and the appropriate QGesture object will be delivered to the
    target as a part of a QGestureEvent.

    \value MaybeGesture The event changed the internal state of the recognizer,
    but it isn't clear yet if it is a gesture or not. The recognizer needs to
    filter more events to decide. Gesture recognizers in the MaybeGesture state
    may be reset automatically if they take too long to recognize gestures.

    \value GestureTriggered The gesture has been triggered and the appropriate
    QGesture object will be delivered to the target as a part of a
    QGestureEvent.

    \value GestureFinished The gesture has been finished successfully and the
    appropriate QGesture object will be delivered to the target as a part of a
    QGestureEvent.

    \value ConsumeEventHint This hint specifies that the gesture framework should
    consume the filtered event and not deliver it to the receiver.

    \omitvalue ResultState_Mask
    \omitvalue ResultHint_Mask

    \sa QGestureRecognizer::filterEvent()
*/

/*!
    Constructs a new gesture recognizer object.
*/
QGestureRecognizer::QGestureRecognizer()
{
}

/*!
    Destroys the gesture recognizer.
*/
QGestureRecognizer::~QGestureRecognizer()
{
}

/*!
    This function is called by Qt to create a new QGesture object for the
    given \a target (QWidget or QGraphicsObject).

    Reimplement this function to create a custom QGesture-derived gesture
    object if necessary.
*/
QGesture *QGestureRecognizer::createGesture(QObject *target)
{
    Q_UNUSED(target);
    return new QGesture;
}

/*!
    This function is called by the framework to reset a given \a gesture.

    Reimplement this function to implement additional requirements for custom QGesture
    objects. This may be necessary if you implement a custom QGesture whose properties
    need special handling when the gesture is reset.
*/
void QGestureRecognizer::reset(QGesture *gesture)
{
    if (gesture) {
        QGesturePrivate *d = gesture->d_func();
        d->state = Qt::NoGesture;
        d->hotSpot = QPointF();
        d->targetObject = 0;
    }
}

/*!
    \fn QGestureRecognizer::filterEvent(QGesture *gesture, QObject *watched, QEvent *event)

    Handles the given \a event for the \a watched object, updating the state of the \a gesture
    object as required, and returns a suitable result for the current recognition step.

    This function is called by the framework to allow the recognizer to filter input events
    dispatched to QWidget or QGraphicsObject instances that it is monitoring.

    The result reflects how much of the gesture has been recognized. The state of the
    \a gesture object is set depending on the result.

    \sa Qt::GestureState
*/

QT_END_NAMESPACE