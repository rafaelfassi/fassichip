var obj = new Object;
obj.UseDialog = true;

obj.Run = function() 
{
	if (!this.initialized)
	{
		this.Dialog.GroupIgn.BtnMapIgn2d.clicked.connect(this, this.Ign2d);
		this.Dialog.GroupIgn.BtnMapIgn3d.clicked.connect(this, this.Ign3d);
                this.Dialog.GroupInj.BtnMapInj2d.clicked.connect(this, this.Inj2d);
                this.Dialog.GroupInj.BtnMapInj3d.clicked.connect(this, this.Inj3d);
                this.Dialog.BtnCalcChkSum.clicked.connect(this, this.ChkSum);
		this.initialized = true;
	}
	this.Dialog.show();
    
};

obj.Ign2d = function() 
{
    	var View2d = new DataObj;
	View2d.StartAddress = 0xAE0;
	View2d.EndAddress = 0xB7F;
	View2d.SelectionX = 16;
	View2d.SelectionY = 10;
	Plugin.Exec("GraphicEditor.Show", View2d);
};

obj.Ign3d = function() 
{
    	var Map3d = new DataObj;
	var SizeX = 16;
	var SizeY = 10;
	var Ini = 0xAE0;
	var OffsetX = 1; //8 Bits OffsetX = 1; 16Bits OffsetX = 2; 32Bits OffsetX = 4;

	SizeX = SizeX/OffsetX;

	Map3d.Data3D.SetSize(SizeX, SizeY);

	for(x=0; x<SizeX; x++)
	{
	   for(y=0; y<SizeY; y++) Map3d.Data3D.SetAddressZ(x, y, Ini + x*OffsetX + y*SizeX*OffsetX);
	}

	Map3d.Data3D.CreateDefaultScale();
	Plugin.Exec("Viewer3d.Show", Map3d);
	};

obj.Inj2d = function() 
{
    	var View2d = new DataObj;
	View2d.StartAddress = 0x840;
	View2d.EndAddress = 0x93F;
	View2d.SelectionX = 16;
	View2d.SelectionY = 16;
	Plugin.Exec("GraphicEditor.Show", View2d);
};

obj.Inj3d = function() 
{
    	var Map3d = new DataObj;
	var SizeX = 16;
	var SizeY = 16;
	var Ini = 0x840;
	var OffsetX = 1; //8 Bits OffsetX = 1; 16Bits OffsetX = 2; 32Bits OffsetX = 4;

	SizeX = SizeX/OffsetX;

	Map3d.Data3D.SetSize(SizeX, SizeY);

	for(x=0; x<SizeX; x++)
	{
	   for(y=0; y<SizeY; y++) Map3d.Data3D.SetAddressZ(x, y, Ini + x*OffsetX + y*SizeX*OffsetX);
	}

	Map3d.Data3D.CreateDefaultScale();
	Plugin.Exec("Viewer3d.Show", Map3d);
};
    
obj.ChkSum = function() 
{
    	var sum16; 
        sum16 = 0;

        Data.FileType = 0;
        Data.ModeBits = "8Bits";
        Data.Signal = false;
        Data.Swap = false;

        for(var x = 0x10; x <= 0x35FF; x++){ sum16 += Data.ReadValue(x); }

        for(var x = 0x3800; x <= 0x5FFF; x++) { sum16 += Data.ReadValue(x); }

        for(var x = 0x6000; x <= 0x7FFF; x++) { sum16 += Data.ReadValue(x); }

        Data.ModeBits = "16Bits";

        sum16 += Data.ReadValue(0x14FE);

        MsgBox.text = Data.To_16Bits(sum16);
        MsgBox.exec();
        this.Dialog.LabChkSum.text = Data.To_16Bits(sum16);
};

obj;
