// Copyright Epic Games, Inc. All Rights Reserved.

//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 4.0.1
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class FDatasmithFacadeMaterialExpressionGeneric : FDatasmithFacadeMaterialExpression {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal FDatasmithFacadeMaterialExpressionGeneric(global::System.IntPtr cPtr, bool cMemoryOwn) : base(DatasmithFacadeCSharpPINVOKE.FDatasmithFacadeMaterialExpressionGeneric_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(FDatasmithFacadeMaterialExpressionGeneric obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  protected override void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          DatasmithFacadeCSharpPINVOKE.delete_FDatasmithFacadeMaterialExpressionGeneric(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public void SetExpressionName(string InExpressionName) {
    DatasmithFacadeCSharpPINVOKE.FDatasmithFacadeMaterialExpressionGeneric_SetExpressionName(swigCPtr, InExpressionName);
  }

  public string GetExpressionName() {
    string ret = global::System.Runtime.InteropServices.Marshal.PtrToStringUni(DatasmithFacadeCSharpPINVOKE.FDatasmithFacadeMaterialExpressionGeneric_GetExpressionName(swigCPtr));
    return ret;
  }

  public int GetPropertiesCount() {
    int ret = DatasmithFacadeCSharpPINVOKE.FDatasmithFacadeMaterialExpressionGeneric_GetPropertiesCount(swigCPtr);
    return ret;
  }

  public void AddProperty(FDatasmithFacadeKeyValueProperty InPropertyPtr) {
    DatasmithFacadeCSharpPINVOKE.FDatasmithFacadeMaterialExpressionGeneric_AddProperty(swigCPtr, FDatasmithFacadeKeyValueProperty.getCPtr(InPropertyPtr));
  }

  public FDatasmithFacadeKeyValueProperty GetProperty(int Index) {
	global::System.IntPtr objectPtr = DatasmithFacadeCSharpPINVOKE.FDatasmithFacadeMaterialExpressionGeneric_GetProperty(swigCPtr, Index);
	if(objectPtr == global::System.IntPtr.Zero)
	{
		return null;
	}
	else
	{
		return new FDatasmithFacadeKeyValueProperty(objectPtr, true);
	}
}

}
