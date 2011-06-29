/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.TagInterface
{
	#region Real
	/// <summary>
	/// Real definition class
	/// </summary>
	/// <remarks>Can be either a Angle, Real, Real Fraction</remarks>
	public sealed class Real : Field
	{
		#region Value
		/// <summary>
		/// The value of this real (in a float)
		/// </summary>
		public float Value = 0.0f;

		/// <summary>
		/// Interfaces with the real value
		/// </summary>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = (float)value; }
		}

		/// <summary>
		/// Calls <c>Value</c>'s <c>Equals</c> method
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public override bool Equals(object obj)	{ return Value.Equals(obj); }
		/// <summary>
		/// Calls <c>Value</c>'s <c>GetHashCode</c> method
		/// </summary>
		/// <returns></returns>
		public override int GetHashCode()		{ return Value.GetHashCode(); }
		/// <summary>
		/// Calls <c>Value</c>'s <c>ToString</c> method
		/// </summary>
		/// <returns></returns>
		public override string ToString()		{ return Value.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real field (Real)
		/// </summary>
		public Real() :									base(FieldType.Real)	{}
		/// <summary>
		/// Construct a real field
		/// </summary>
		/// <param name="t">specific real type</param>
		public Real(FieldType t) :						base(t)					{}
		/// <summary>
		/// Construct a real field
		/// </summary>
		/// <param name="t">specific real type</param>
		/// <param name="value">field value</param>
		public Real(FieldType t, float value) :			base(t)					{ Value = value; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public Real(Real value) :						base(value.fieldType)	{ this.Value = value; }
		/// <summary>
		/// Construct a real field (Real)
		/// </summary>
		/// <param name="value">field value</param>
		public Real(float value) :						base(FieldType.Real)	{ this.Value = value; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)						{ return new Real( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()											{ return new Real(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a Real field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling)	{ (args.FieldInterface as Editors.IReal).Field = this.Value; }
			else				{ this.Value = (args.FieldInterface as Editors.IReal).Field; }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			if(this.fieldType == FieldType.Real || this.fieldType == FieldType.RealFraction)	Value = er.ReadSingle();
			else																				Value = RadiansToDegrees(er.ReadSingle()); // angle
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			if (this.fieldType == FieldType.Real || this.fieldType == FieldType.RealFraction)	ew.Write(Value);
			else																				ew.Write(DegreesToRadians(Value)); // angle
		}
		#endregion

		#region Util
		/// <summary>
		/// Converts a degree to radians
		/// </summary>
		/// <param name="angle"></param>
		/// <returns></returns>
		public static float DegreesToRadians(float angle)
		{
			if (angle == (float)0) return 0;

			float radians = /*(System.Math.PI / 180.0f)*/ 0.017453292f * angle;
			return (float)radians;
		}

		/// <summary>
		/// Converts a radian to degrees
		/// </summary>
		/// <param name="radian"></param>
		/// <returns></returns>
		public static float RadiansToDegrees(float radian)
		{
			if (radian == (float)0) return 0;

			float degrees = /*(180.0f / System.Math.PI)*/ 57.29578f * radian;
			return degrees;
		}

		/// <summary>
		/// Returns a new Real field with angle attributes
		/// </summary>
		public static Real Angle { get { return new Real(FieldType.Angle); } }
		/// <summary>
		/// Returns a new Real field with real_fraction attributes
		/// </summary>
		public static Real Fraction { get { return new Real(FieldType.RealFraction); } }
		#endregion

		#region Operators
		#region Conversions
		/// <summary>
		/// Implicit cast to a single precision value
		/// </summary>
		/// <param name="value">field being casted</param>
		/// <returns>field's value</returns>
		public static implicit operator float(Real value) { return value.Value; }
		/// <summary>
		/// Implicit cast from a floating-point to a real field
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>real field</returns>
		public static implicit operator Real(float value) { return new Real(value); }
		#endregion

		#region Boolean
		/// <summary>
		/// Compare two fields (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(Real lhs, Real rhs) { return lhs.Value == rhs.Value; }
		/// <summary>
		/// Compare two fields (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(Real lhs, Real rhs) { return lhs.Value != rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt;= <paramref name="rhs"/></returns>
		public static bool operator >=(Real lhs, Real rhs) { return lhs.Value >= rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than or equal)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt;= <paramref name="rhs"/></returns>
		public static bool operator <=(Real lhs, Real rhs) { return lhs.Value <= rhs.Value; }
		/// <summary>
		/// Compare two fields (greater-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &gt; <paramref name="rhs"/></returns>
		public static bool operator >(Real lhs, Real rhs) { return lhs.Value > rhs.Value; }
		/// <summary>
		/// Compare two fields (less-than)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> &lt; <paramref name="rhs"/></returns>
		public static bool operator <(Real lhs, Real rhs) { return lhs.Value < rhs.Value; }
		#endregion

		#region Math
		/// <summary>
		/// Perform mathematical operation (Add)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> + <paramref name="rhs"/></returns>
		public static float operator +(Real lhs, Real rhs) { return lhs.Value + rhs.Value; }
		/// <summary>
		/// Perform mathematical operation (Subtract)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> - <paramref name="rhs"/></returns>
		public static float operator -(Real lhs, Real rhs) { return lhs.Value - rhs.Value; }
		/// <summary>
		/// Perform mathematical operation (Multiply)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> * <paramref name="rhs"/></returns>
		public static float operator *(Real lhs, Real rhs) { return lhs.Value * rhs.Value; }
		/// <summary>
		/// Perform mathematical operation (Divide)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> / <paramref name="rhs"/></returns>
		public static float operator /(Real lhs, Real rhs) { return lhs.Value / rhs.Value; }
		#endregion
		#endregion
	};
	#endregion

	#region Real Point 2D
	/// <summary>
	/// Real Point 2D definition class; X, Y
	/// </summary>
	public sealed class RealPoint2D : Field
	{
		#region Value
		/// <summary>
		/// X value of this field
		/// </summary>
		public float X = 0.0f;
		/// <summary>
		/// Y value of this field
		/// </summary>
		public float Y = 0.0f;

		/// <summary>
		/// float x, y
		/// </summary>
		public float[] Value
		{
			get { return new float[] { X, Y }; }
			set { X = value[0]; Y = value[1]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: X and Y
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>X @ 0</item>
		/// <item>Y @ 1</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real point2d field
		/// </summary>
		public RealPoint2D() :							base(FieldType.RealPoint2D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealPoint2D(RealPoint2D value) :			this()						{ X = value.X; Y = value.Y; }
		/// <summary>
		/// Construct a real point2d field
		/// </summary>
		/// <param name="x">field value (X)</param>
		/// <param name="y">field value (Y)</param>
		public RealPoint2D(float x, float y) :			this()						{ X = x; Y = y; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new RealPoint2D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new RealPoint2D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealPoint2D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			X = er.ReadSingle();
			Y = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(X);
			ew.Write(Y);
		}
		#endregion
	};
	#endregion

	#region Real Point 3D
	/// <summary>
	/// Real Point 3D definition class; X, Y, Z
	/// </summary>
	public sealed class RealPoint3D : Field
	{
		#region Value
		/// <summary>
		/// X value of this field
		/// </summary>
		public float X = 0.0f;
		/// <summary>
		/// Y value of this field
		/// </summary>
		public float Y = 0.0f;
		/// <summary>
		/// Z value of this field
		/// </summary>
		public float Z = 0.0f;

		/// <summary>
		/// float x, y, z
		/// </summary>
		public float[] Value
		{
			get { return new float[] { X, Y, Z }; }
			set { X = value[0]; Y = value[1]; Z = value[2]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: X, Y, and Z
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>X @ 0</item>
		/// <item>Y @ 1</item>
		/// <item>Z @ 2</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real point3d field
		/// </summary>
		public RealPoint3D() :							base(FieldType.RealPoint3D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealPoint3D(RealPoint3D value) :			this()						{ X = value.X; Y = value.Y; Z = value.Z; }
		/// <summary>
		/// Construct a real point3d field
		/// </summary>
		/// <param name="x">field value (X)</param>
		/// <param name="y">field value (Y)</param>
		/// <param name="z">field value (Z)</param>
		public RealPoint3D(float x, float y, float z) :	this()						{ X = x; Y = y; Z = z; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)							{ return new RealPoint3D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()												{ return new RealPoint3D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealPoint3D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			X = er.ReadSingle();
			Y = er.ReadSingle();
			Z = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(X);
			ew.Write(Y);
			ew.Write(Z);
		}
		#endregion
	};
	#endregion

	#region Real Vector 2D
	/// <summary>
	/// Real Vector 2D definition class; I, J
	/// </summary>
	public sealed class RealVector2D : Field
	{
		#region Value
		/// <summary>
		/// I value of this field
		/// </summary>
		public float I = 0.0f;
		/// <summary>
		/// J value of this field
		/// </summary>
		public float J = 0.0f;

		/// <summary>
		/// float i, j
		/// </summary>
		public float[] Value
		{
			get { return new float[] { I, J }; }
			set { I = value[0]; J = value[1]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: I and J
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>I @ 0</item>
		/// <item>J @ 1</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}

		/// <summary>
		/// Normalizes the vector
		/// </summary>
		public void Normalize()
		{
			float mag = (float)Math.Sqrt(
				I * I +
				J * J);

			I /= mag;
			J /= mag;
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real vector2d field
		/// </summary>
		public RealVector2D() :							base(FieldType.RealVector2D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealVector2D(RealVector2D value) :		this()							{ I = value.I; J = value.J; }
		/// <summary>
		/// Construct a real vector2d field
		/// </summary>
		/// <param name="i">field value (I)</param>
		/// <param name="j">field value (J)</param>
		public RealVector2D(float i, float j) :			this()							{ I = i; J = j; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)								{ return new RealVector2D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()													{ return new RealVector2D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealVector2D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			I = er.ReadSingle();
			J = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(I);
			ew.Write(J);
		}
		#endregion
	};
	#endregion

	#region Real Vector 3D
	/// <summary>
	/// Real Vector 3D definition class; I, J, K
	/// </summary>
	public sealed class RealVector3D : Field
	{
		#region Value
		/// <summary>
		/// I value of this field
		/// </summary>
		public float I = 0.0f;
		/// <summary>
		/// J value of this field
		/// </summary>
		public float J = 0.0f;
		/// <summary>
		/// K value of this field
		/// </summary>
		public float K = 0.0f;

		/// <summary>
		/// float i, j, k
		/// </summary>
		public float[] Value
		{
			get { return new float[] { I, J, K }; }
			set { I = value[0]; J = value[1]; K = value[2]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: I, J, and K
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>I @ 0</item>
		/// <item>J @ 1</item>
		/// <item>K @ 2</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}

		/// <summary>
		/// Normalizes the vector
		/// </summary>
		public void Normalize()
		{
			float mag = (float)Math.Sqrt(
				I * I +
				J * J +
				K * K);

			I /= mag;
			J /= mag;
			K /= mag;
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real vector3d field
		/// </summary>
		public RealVector3D() :								base(FieldType.RealVector3D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealVector3D(RealVector3D value) :			this()							{ I = value.I; J = value.J; K = value.K; }
		/// <summary>
		/// Construct a real vector3d field
		/// </summary>
		/// <param name="i">field value (I)</param>
		/// <param name="j">field value (J)</param>
		/// <param name="k">field value (K)</param>
		public RealVector3D(float i, float j, float k) :	this()							{ I = i; J = j; K = k; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)									{ return new RealVector3D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()														{ return new RealVector3D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealVector2D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			I = er.ReadSingle();
			J = er.ReadSingle();
			K = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(I);
			ew.Write(J);
			ew.Write(K);
		}
		#endregion
	};
	#endregion

	#region Real Quaternion
	/// <summary>
	/// Real Quaternion definition class; I, J, K, W
	/// </summary>
	public sealed class RealQuaternion : Field
	{
		#region Value
		/// <summary>
		/// I value of this field
		/// </summary>
		public float I = 0.0f;
		/// <summary>
		/// J value of this field
		/// </summary>
		public float J = 0.0f;
		/// <summary>
		/// K value of this field
		/// </summary>
		public float K = 0.0f;
		/// <summary>
		/// W value of this field
		/// </summary>
		public float W = 0.0f;

		/// <summary>
		/// float i, j, k, w
		/// </summary>
		public float[] Value
		{
			get { return new float[] { I, J, K, W }; }
			set { I = value[0]; J = value[1]; K = value[2]; W = value[3]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: I, J, K, and W
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>I @ 0</item>
		/// <item>J @ 1</item>
		/// <item>K @ 2</item>
		/// <item>W @ 3</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}

		/// <summary>
		/// Converts the quaternion into an euler rotation
		/// </summary>
		/// <returns>Returns an euler rotation</returns>
		public LowLevel.Math.real_euler_angles3d ToEuler3D()
		{
			var euler_3d = new LowLevel.Math.real_euler_angles3d();

			float x = -I;
			float y = -J;
			float z = -K;
			float w = W;

			if ((x * y) + (z * w) == 0.5f)
			{
				euler_3d.Yaw = (float)(2 * Math.Atan2(x, w));
				euler_3d.Roll = 0;
			}
			else if ((x * y) + (z * w) == -0.5f)
			{
				euler_3d.Yaw = (float)(-2 * Math.Atan2(x, w));
				euler_3d.Roll = 0;
			}
			else
			{
				euler_3d.Yaw = (float)Math.Atan2(
					2 * y * w - 2 * x * z,
					1 - 2 * (y * y) - 2 * (z * z));
				euler_3d.Roll = (float)Math.Atan2(
					2 * x * w - 2 * y * z,
					1 - 2 * (x * x) - 2 * (z * z));
			}
			euler_3d.Pitch = (float)Math.Asin(2 * x * y + 2 * z * w);

			euler_3d.Yaw = Real.RadiansToDegrees(euler_3d.Yaw);
			euler_3d.Pitch = Real.RadiansToDegrees(euler_3d.Pitch);
			euler_3d.Roll = Real.RadiansToDegrees(euler_3d.Roll);

			return euler_3d;
		}

		/// <summary>
		/// Normalizes the quaternion
		/// </summary>
		public void Normalize()
		{
			float mag = (float)Math.Sqrt(
				I * I + 
				J * J +
				K * K +
				W * W);

			I /= mag;
			J /= mag;
			K /= mag;
			W /= mag;
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real quaternion field
		/// </summary>
		public RealQuaternion() :									base(FieldType.RealQuaternion)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealQuaternion(RealQuaternion value) :				this()							{ I = value.I; J = value.J; K = value.K; W = value.W; }
		/// <summary>
		/// Construct a real quaternion field
		/// </summary>
		/// <param name="i">field value (I)</param>
		/// <param name="j">field value (J)</param>
		/// <param name="k">field value (K)</param>
		/// <param name="w">field value (W)</param>
		public RealQuaternion(float i, float j, float k, float w) :	this()							{ I = i; J = j; K = k; W = w; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)											{ return new RealQuaternion(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()																{ return new RealQuaternion(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealQuaternion field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			I = er.ReadSingle();
			J = er.ReadSingle();
			K = er.ReadSingle();
			W = er.ReadSingle();

			Normalize();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			Normalize();

			ew.Write(I);
			ew.Write(J);
			ew.Write(K);
			ew.Write(W);
		}
		#endregion
	};
	#endregion

	#region Real Euler Angles 2D
	/// <summary>
	/// Real Euler Angles 2D definition class; Yaw, Pitch
	/// </summary>
	public sealed class RealEulerAngles2D : Field
	{
		#region Value
		/// <summary>
		/// Yaw value of this field
		/// </summary>
		public float Y = 0.0f;
		/// <summary>
		/// Pitch value of this field
		/// </summary>
		public float P = 0.0f;

		/// <summary>
		/// float y, p
		/// </summary>
		public float[] Value
		{
			get { return new float[] { Y, P }; }
			set { Y = value[0]; P = value[1]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: Y and P
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>Y @ 0</item>
		/// <item>P @ 1</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real euler angles2d field
		/// </summary>
		public RealEulerAngles2D() :						base(FieldType.RealEulerAngles2D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealEulerAngles2D(RealEulerAngles2D value) :	this()								{ Y = value.Y; P = value.P; }
		/// <summary>
		/// Construct a real euler angles2d field
		/// </summary>
		/// <param name="y">field value (Y)</param>
		/// <param name="p">field value (P)</param>
		public RealEulerAngles2D(float y, float p) :		this()								{ Y = y; P = p; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new RealEulerAngles2D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new RealEulerAngles2D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealEulerAngles2D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			Y = Real.RadiansToDegrees(er.ReadSingle());
			P = Real.RadiansToDegrees(er.ReadSingle());
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(Real.DegreesToRadians(Y));
			ew.Write(Real.DegreesToRadians(P));
		}
		#endregion
	};
	#endregion

	#region Real Euler Angles 3D
	/// <summary>
	/// Real Euler Angles 3D definition class; Yaw, Pitch, Roll
	/// </summary>
	public sealed class RealEulerAngles3D : Field
	{
		#region Value
		/// <summary>
		/// Yaw value of this field
		/// </summary>
		public float Y = 0.0f;
		/// <summary>
		/// Pitch value of this field
		/// </summary>
		public float P = 0.0f;
		/// <summary>
		/// Roll value of this field
		/// </summary>
		public float R = 0.0f;

		/// <summary>
		/// float y, p, r
		/// </summary>
		public float[] Value
		{
			get { return new float[] { Y, P, R }; }
			set { Y = value[0]; P = value[1]; R = value[2]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: Y, P, and R
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>Y @ 0</item>
		/// <item>P @ 1</item>
		/// <item>R @ 2</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real euler angles3d field
		/// </summary>
		public RealEulerAngles3D() :							base(FieldType.RealEulerAngles3D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealEulerAngles3D(RealEulerAngles3D value) :		this()								{ Y = value.Y; P = value.P; R = value.R; }
		/// <summary>
		/// Construct a real euler angles3d field
		/// </summary>
		/// <param name="y">field value (Y)</param>
		/// <param name="p">field value (P)</param>
		/// <param name="r">field value (R)</param>
		public RealEulerAngles3D(float y, float p, float r) :	this()								{ Y = y; P = p; R = r; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)											{ return new RealEulerAngles3D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()																{ return new RealEulerAngles3D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealEulerAngles3D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			Y = Real.RadiansToDegrees(er.ReadSingle());
			P = Real.RadiansToDegrees(er.ReadSingle());
			R = Real.RadiansToDegrees(er.ReadSingle());
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(Real.DegreesToRadians(Y));
			ew.Write(Real.DegreesToRadians(P));
			ew.Write(Real.DegreesToRadians(R));
		}
		#endregion
	};
	#endregion

	#region Real Plane 2D
	/// <summary>
	/// Real Plane 2D definition class; I, J, D
	/// </summary>
	public sealed class RealPlane2D : Field
	{
		#region Value
		/// <summary>
		/// I value of this field
		/// </summary>
		public float I = 0.0f;
		/// <summary>
		/// J value of this field
		/// </summary>
		public float J = 0.0f;
		/// <summary>
		/// D value of this field
		/// </summary>
		public float D = 0.0f;

		/// <summary>
		/// float i, j, d
		/// </summary>
		public float[] Value
		{
			get { return new float[] { I, J, D }; }
			set { I = value[0]; J = value[1]; D = value[2]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: I, J, and D
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>I @ 0</item>
		/// <item>J @ 1</item>
		/// <item>D @ 2</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real plane2d field
		/// </summary>
		public RealPlane2D() :								base(FieldType.RealPlane2D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealPlane2D(RealPlane2D value) :				this()						{ I = value.I; J = value.J; D = value.D; }
		/// <summary>
		/// Construct a real plane2d field
		/// </summary>
		/// <param name="i">field value (I)</param>
		/// <param name="j">field value (J)</param>
		/// <param name="d">field value (D)</param>
		public RealPlane2D(float i, float j, float d) :		this()						{ I = i; J = j; D = d; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)								{ return new RealPlane2D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()													{ return new RealPlane2D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealPlane2D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			I = er.ReadSingle();
			J = er.ReadSingle();
			D = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(I);
			ew.Write(J);
			ew.Write(D);
		}
		#endregion
	};
	#endregion

	#region Real Plane 3D
	/// <summary>
	/// Real Plane 3D definition class; I, J, D, K
	/// </summary>
	public sealed class RealPlane3D : Field
	{
		#region Value
		/// <summary>
		/// I value of this field
		/// </summary>
		public float I = 0.0f;
		/// <summary>
		/// J value of this field
		/// </summary>
		public float J = 0.0f;
		/// <summary>
		/// D value of this field
		/// </summary>
		public float D = 0.0f;
		/// <summary>
		/// K value of this field
		/// </summary>
		public float K = 0.0f;

		/// <summary>
		/// float i, j, d, k
		/// </summary>
		public float[] Value
		{
			get { return new float[] { I, J, D, K }; }
			set { I = value[0]; J = value[1]; D = value[2]; K = value[3]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: I, J, D, and K
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>I @ 0</item>
		/// <item>J @ 1</item>
		/// <item>D @ 2</item>
		/// <item>K @ 3</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real plane3d field
		/// </summary>
		public RealPlane3D() :										base(FieldType.RealPlane3D)	{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealPlane3D(RealPlane3D value) :						this()						{ I = value.I; J = value.J; D = value.D; K = value.K; }
		/// <summary>
		/// Construct a real plane3d field
		/// </summary>
		/// <param name="i">field value (I)</param>
		/// <param name="j">field value (J)</param>
		/// <param name="d">field value (D)</param>
		/// <param name="k">field value (K)</param>
		public RealPlane3D(float i, float j, float d, float k) :	this()						{ I = i; J = j; D = d; K = d; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field (unused)</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new RealPlane3D(); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new RealPlane3D(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealPlane3D field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			I = er.ReadSingle();
			J = er.ReadSingle();
			D = er.ReadSingle();
			K = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			ew.Write(I);
			ew.Write(J);
			ew.Write(D);
			ew.Write(K);
		}
		#endregion
	};
	#endregion

	#region Real Color
	/// <summary>
	/// Real Color definition class; A, R, G, B
	/// </summary>
	public sealed class RealColor : Field
	{
		#region Value
		/// <summary>
		/// Alpha value of this field
		/// </summary>
		public float A = 0.0f;
		/// <summary>
		/// Red value of this field
		/// </summary>
		public float R = 0.0f;
		/// <summary>
		/// Green value of this field
		/// </summary>
		public float G = 0.0f;
		/// <summary>
		/// Blue value of this field
		/// </summary>
		public float B = 0.0f;

		/// <summary>
		/// float a, r, g, b
		/// </summary>
		public float[] Value
		{
			get { return new float[] { A, R, G, B }; }
			set { A = value[0]; R = value[1]; G = value[2]; B = value[3]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: A, R, G, and B
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>A @ 0</item>
		/// <item>R @ 1</item>
		/// <item>G @ 2</item>
		/// <item>B @ 3</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real Rgb Color field
		/// </summary>
		public RealColor() :									base(FieldType.RealRgbColor)	{}
		/// <summary>
		/// Construct a real color field
		/// </summary>
		/// <param name="t">specific real color type</param>
		public RealColor(FieldType t) :							base(t)							{}
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealColor(RealColor value) :						this()							{ A = value.A; R = value.R; G = value.G; B = value.B; }
		/// <summary>
		/// Construct a real argb color field
		/// </summary>
		/// <param name="a">field value (A)</param>
		/// <param name="r">field value (R)</param>
		/// <param name="g">field value (G)</param>
		/// <param name="b">field value (B)</param>
		public RealColor(float a, float r, float g, float b) :	this()							{ A = a; B = b; R = r; B = b; }
		/// <summary>
		/// Construct a real rgb color field
		/// </summary>
		/// <param name="r">field value (R)</param>
		/// <param name="g">field value (G)</param>
		/// <param name="b">field value (B)</param>
		public RealColor(float r, float g, float b) :			this()							{ A = 0; B = b; R = r; B = b; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new RealColor( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new RealColor(this); }
		#endregion

		#region Util
		/// <summary>
		/// Convert to a rgb color field
		/// </summary>
		/// <returns></returns>
		public Color ToRgbColor()								{ return new Color(					(byte)(R * 255), (byte)(G * 255), (byte)(B * 255)); }
		/// <summary>
		/// Convert to a argb color field
		/// </summary>
		/// <returns></returns>
		public Color ToArgbColor()								{ return new Color((byte)(A * 255),	(byte)(R * 255), (byte)(G * 255), (byte)(B * 255)); }
		/// <summary>
		/// Convert from rgb color field values
		/// </summary>
		/// <param name="r">Red value</param>
		/// <param name="g">Green value</param>
		/// <param name="b">Blue value</param>
		public void ToRgbColor(byte r, byte g, byte b)			{ A = 0;						R = (r != 0 ? r / 255 : 0); G = (g != 0 ? g / 255 : 0); B = (b != 0 ? b / 255 : 0); }
		/// <summary>
		/// Convert from argb color field values
		/// </summary>
		/// <param name="a">Alpha value</param>
		/// <param name="r">Red value</param>
		/// <param name="g">Green value</param>
		/// <param name="b">Blue value</param>
		public void ToArgbColor(byte a, byte r, byte g, byte b)	{ A = (a != 0 ? a / 255 : 0);	R = (r != 0 ? r / 255 : 0); G = (g != 0 ? g / 255 : 0); B = (b != 0 ? b / 255 : 0); }
		/// <summary>
		/// Convert to a .NET color object
		/// </summary>
		/// <returns>a new system color object using this field's values</returns>
		public System.Drawing.Color ToColor() { return System.Drawing.Color.FromArgb((int)(A != 0 ? A / 255 : 0), (int)(R != 0 ? R / 255 : 0), (int)(G != 0 ? G / 255 : 0), (int)(B != 0 ? B / 255 : 0)); }

		/// <summary>
		/// Returns a new RealColor field with rgb_color attributes
		/// </summary>
		public static RealColor Rgb { get { return new RealColor(); } }
		/// <summary>
		/// Returns a new RealColor field with argb_color attributes
		/// </summary>
		public static RealColor Argb { get { return new RealColor(FieldType.RealArgbColor); } }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealColor field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			if (this.fieldType == FieldType.RealArgbColor) A = er.ReadSingle();
			R = er.ReadSingle();
			G = er.ReadSingle();
			B = er.ReadSingle();
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			if (this.fieldType == FieldType.RealArgbColor) ew.Write(A);
			ew.Write(R);
			ew.Write(G);
			ew.Write(B);
		}
		#endregion
	};
	#endregion

	#region Real Bounds
	/// <summary>
	/// Real Bounds definition class; Lower, Upper
	/// </summary>
	/// <remarks>an be either a Angle, Real, Real Fraction bounds</remarks>
	public sealed class RealBounds : Field
	{
		#region Value
		/// <summary>
		/// Lower bound of this field
		/// </summary>
		public float Lower = 0.0f;
		/// <summary>
		/// Upper bound of this field
		/// </summary>
		public float Upper = 0.0f;

		/// <summary>
		/// float lower, upper
		/// </summary>
		public float[] Value
		{
			get { return new float[] { Lower, Upper }; }
			set { Lower = value[0]; Upper = value[1]; }
		}
		/// <summary>
		/// Interfaces with a float[] built of: Lower and Upper
		/// </summary>
		/// <remarks>
		/// <list type="bullet">
		/// <item>Lower @ 0</item>
		/// <item>Upper @ 1</item>
		/// </list>
		/// </remarks>
		public override object FieldValue
		{
			get { return Value; }
			set { Value = value as float[]; }
		}

		/// <summary>
		/// This field as a string
		/// </summary>
		/// <returns>"<c>Lower</c> to <c>Upper</c>"</returns>
		public override string ToString() { return Lower.ToString() + " to " + Upper.ToString(); }
		#endregion

		#region Construction
		/// <summary>
		/// Construct a real bounds field
		/// </summary>
		public RealBounds() :										base(FieldType.RealBounds)	{}
		/// <summary>
		/// Construct a real bounds field
		/// </summary>
		/// <param name="t">specific real bounds type</param>
		public RealBounds(FieldType t) :							base(t)						{}
		/// <summary>
		/// Construct a real bounds field
		/// </summary>
		/// <param name="t">specific real bounds type</param>
		/// <param name="lower">field value (Lower)</param>
		/// <param name="upper">field value (Upper)</param>
		public RealBounds(FieldType t, float lower, float upper) :	base(t)						{ this.Lower = lower; this.Upper = upper; }
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="value">field to copy</param>
		public RealBounds(RealBounds value) :						base(value.fieldType)		{ this.Lower = value.Lower; this.Upper = value.Upper; }

		/// <summary>
		/// Creates a object that inherits from Field, using <paramref name="from"/> to pull any needed extra data.
		/// </summary>
		/// <param name="from">reference field</param>
		/// <returns>new field of this type</returns>
		public override Field CreateInstance(Field from)										{ return new RealBounds( from.FieldType ); }
		/// <summary>
		/// Performs a deep copy of this field
		/// </summary>
		/// <returns>Complete copy guarenteed not to reference any tag data which we copied</returns>
		public override object Clone()															{ return new RealBounds(this); }
		#endregion

		#region Data Exchange
		/// <summary>
		/// Exchanges data with a RealBounds field control
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="args"></param>
		public override void DoDataExchange(object sender, Editors.DoDataExchangeEventArgs args)
		{
			if (args.IsPolling) { }
			else { }
		}
		#endregion

		#region I\O
		/// <summary>
		/// Stream the field from a buffer
		/// </summary>
		/// <param name="er"></param>
		public override void Read(IO.EndianReader er)
		{
			if (this.fieldType == FieldType.AngleBounds)
			{
				Lower = Real.RadiansToDegrees(er.ReadSingle());
				Upper = Real.RadiansToDegrees(er.ReadSingle());
			}
			else
			{
				Lower = er.ReadSingle();
				Upper = er.ReadSingle();
			}
		}
		/// <summary>
		/// Stream the field to a buffer
		/// </summary>
		/// <param name="ew"></param>
		public override void Write(IO.EndianWriter ew)
		{
			if (this.fieldType == FieldType.AngleBounds)
			{
				ew.Write(Real.DegreesToRadians(Lower));
				ew.Write(Real.DegreesToRadians(Upper));
			}
			else
			{
				ew.Write(Lower);
				ew.Write(Upper);
			}
		}
		#endregion

		#region Util
		/// <summary>
		/// Returns a new RealBounds field with angle attributes
		/// </summary>
		public static RealBounds Angle { get { return new RealBounds(FieldType.AngleBounds); } }
		/// <summary>
		/// Returns a new RealBounds field with real_fraction attributes
		/// </summary>
		public static RealBounds Fraction { get { return new RealBounds(FieldType.RealFractionBounds); } }
		#endregion
	};
	#endregion
}