var obj = new Object;
obj.UseDialog = true;

obj.Run = function() {
	if (!this.initialized)
	{
		this.Dialog.pushButton.clicked.connect(this, this.apply);
		this.initialized = true;
	}
	this.Dialog.show();
    
};

obj.apply = function() {
    this.Dialog.label.text = this.Dialog.lineEdit.text;
};

obj;
