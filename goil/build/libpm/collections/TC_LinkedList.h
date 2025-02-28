//----------------------------------------------------------------------------------------------------------------------
//
//  Linked list template class.
//
//  COPY OF ITS INSTANCES IS FORBIDDEN BY REDEFINITION OF COPY CONSTRUCTOR
//  AND ASSIGNMENT OPERATOR.
//
//  This file is part of libpm library
//
//  Copyright (C) 2001 Pierre Molinaro.
//  e-mail : pierre@pcmolinaro.name
//  This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
//  Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//
//  This program is distributed in the hope it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//  warranty of MERCHANDIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
//----------------------------------------------------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "utilities/M_machine.h"
#include "utilities/MF_MemoryControl.h"

//----------------------------------------------------------------------------------------------------------------------
//
//   Predeclarations
//
//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> class TC_LinkedList ;

template <typename TYPE> void swap (TC_LinkedList <TYPE> & ioOperand1,
                                    TC_LinkedList <TYPE> & ioOperand2) ;

//----------------------------------------------------------------------------------------------------------------------
//
//   Class template declaration
//
//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> class TC_LinkedList {
//--- Constructor and destructor
  public: TC_LinkedList (void) ;
  public: virtual ~TC_LinkedList (void) ;

//--- No copy
  private: TC_LinkedList (TC_LinkedList <TYPE> &) ;
  private: void operator = (TC_LinkedList <TYPE> &) ;

//--- Suppress all elements
  public: void makeListEmpty (void) ;

//--- Insert a new element
  public: void insertAtTop (const TYPE & inInfo) ;
  public: void insertAtBottom (const TYPE & inInfo) ;

//--- Get and suppress last element
  public: TYPE getByCopyAndSuppressTopItem (LOCATION_ARGS) ;

//--- Merge two lists
  public: void mergeListAtTop (TC_LinkedList <TYPE> & ioList) ;
  public: void mergeListAtBottom (TC_LinkedList <TYPE> & ioList) ;

//--- List empty ?
  public: bool isListEmpty (void) const { return mTopItem == nullptr ; }

//--- Exchange
  friend void swap <TYPE> (TC_LinkedList <TYPE> & ioOperand1,
                           TC_LinkedList <TYPE> & ioOperand2) ;

//------- Element class ---------------------
  private: class cElement {
    public: TYPE mInfo ;
    public: cElement * mNextItem ;
  } ;

//--- Data members
  private: cElement * mTopItem ;
  private: cElement * mBottomItem ;
  private: int32_t mCount ;

//--- Copy into an array
  public: void copyIntoArray (TC_UniqueArray <TYPE> & outArray) ;
} ;

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE>  TC_LinkedList <TYPE>::TC_LinkedList (void) :
mTopItem ((cElement *) nullptr),
mBottomItem ((cElement *) nullptr),
mCount (0) {
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> TC_LinkedList <TYPE>::~TC_LinkedList (void) {
  makeListEmpty () ;
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> void TC_LinkedList <TYPE>::makeListEmpty (void) {
  while (mTopItem != nullptr) {
    mBottomItem = mTopItem->mNextItem ;
    macroMyDelete (mTopItem) ;
    mTopItem = mBottomItem ;
  }
  mCount = 0 ;
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> void TC_LinkedList <TYPE>::insertAtBottom (const TYPE & inInfo) {
  cElement * p = (cElement *) nullptr ;
  macroMyNew (p, cElement) ;
  p->mNextItem = (cElement *) nullptr ;
  p->mInfo = inInfo ; // Copy
  if (mTopItem == nullptr) {
    mTopItem = p ;
  }else{
    mBottomItem->mNextItem = p ;
  }
  mBottomItem = p ;
  mCount ++ ;
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> void TC_LinkedList<TYPE> ::insertAtTop (const TYPE & inInfo) {
  cElement * p = (cElement *) nullptr ;
  macroMyNew (p, cElement) ;
  p->mNextItem = mTopItem ;
  p->mInfo = inInfo ; // Copy
  mTopItem = p ;
  if (mBottomItem == nullptr) {
    mBottomItem = p ;
  }
  mCount ++ ;
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> TYPE TC_LinkedList <TYPE>::getByCopyAndSuppressTopItem (LOCATION_ARGS) {
  MF_AssertThere (mTopItem != nullptr, "mTopItem == nullptr", 0, 0) ;
  TYPE info (mTopItem->mInfo) ; // Copy constructor call
  cElement * p = mTopItem->mNextItem ;
  macroMyDelete (mTopItem) ;
  mTopItem = p ;
  if (mTopItem == nullptr) {
    mBottomItem = (cElement *) nullptr ;
  }
  mCount -- ;
  return info ;
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> void TC_LinkedList <TYPE>::mergeListAtTop (TC_LinkedList <TYPE> & ioList) {
  MF_Assert (this != & ioList, "this == & ioList", 0, 0) ;
  if (ioList.mTopItem != nullptr) {
    if (mBottomItem == nullptr) {
      mBottomItem = ioList.mBottomItem ;
    }else{
      ioList.mBottomItem->mNextItem = mTopItem ;
    }
    mTopItem = ioList.mTopItem ;
    ioList.mTopItem = (cElement *) nullptr ;
    ioList.mBottomItem = (cElement *) nullptr ;
    mCount += ioList.mCount ;
    ioList.mCount = 0 ;
  }
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> void TC_LinkedList <TYPE>::mergeListAtBottom (TC_LinkedList <TYPE> & ioList) {
  MF_Assert (this != & ioList, "this == & ioList", 0, 0) ;
  if (ioList.mTopItem != nullptr) {
    if (mBottomItem == nullptr) {
      mTopItem = ioList.mTopItem ;
    }else{
      mBottomItem->mNextItem = ioList.mTopItem ;
    }
    mBottomItem = ioList.mBottomItem ;
    ioList.mTopItem = (cElement *) nullptr ;
    ioList.mBottomItem = (cElement *) nullptr ;
    mCount += ioList.mCount ;
    ioList.mCount = 0 ;
  }
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE> void TC_LinkedList <TYPE>::copyIntoArray (TC_UniqueArray <TYPE> & outArray) {
  outArray.free () ;
  outArray.setCapacity (mCount) ;
  cElement * p = mTopItem ;
  while (p != nullptr) {
    outArray.appendObject (p->mInfo) ;
    p = p->mNextItem ;
  }
}

//----------------------------------------------------------------------------------------------------------------------

template <typename TYPE>void swap (TC_LinkedList <TYPE> & ioList1,
                                   TC_LinkedList <TYPE> & ioList2) {
  swap (ioList1.mTopItem, ioList2.mTopItem) ;
  swap (ioList1.mBottomItem, ioList2.mBottomItem) ;
  swap (ioList1.mCount, ioList2.mCount) ;
}

//----------------------------------------------------------------------------------------------------------------------
