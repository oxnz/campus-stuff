var MM;
function onLoad() {
	MM = new MManager();
	if (!MM) {
		alert("Error: can't create Memory Manager!");
		return false;
	}
	else {
		var console = document.getElementById('console');
		if (console) {
			console.value = 'log:\n';
			MM.console = console;
		}
		else
		{
			alert("Error: can't find console element!");
		}
		var btn = document.getElementById('mallocBtn');
		if (btn)
			MM.mallocBtn = btn;
		else
			MM.console.value += "Can't find malloc button\n";
		btn = document.getElementById('freeBtn');
		if (btn)
			MM.freeBtn = btn;
		else
			MM.console.value += "Can't find free button\n";
		var tbl = document.getElementById('mTable');
		if (tbl)
			MM.table = tbl;
		else
			MM.console.value += "Can't find memory table\n";
	}
}
function onBeforeUnload() {
	delete MM;
}
function MSlice(uuid, size, used) {
	if (size < 1)
		return "invalid size";
	this.uuid = uuid;
	this.size = size;
	this.used = used;
	this.prev = null;
	this.next = null;
//	MM.console.value += '>>uuid=' + this.uuid + 'size=' + this.size + 'used=' + this.used + '\n';
}
function MManager() {
	this.uuid = 0;
	this.size = 0;
	this.avail = 0;
	this.used = 0;
	this.mallocBtn = null;
	this.freeBtn = null;
	this.console = null;
	this.pool = null;
	this.table = null;

	this.setSize = function() {
		this.uuid = 0;
		this.avail = 0;
		this.used = 0;
		var size = parseInt(document.getElementById('mSize').value);
		if (size > 0) {
			this.size = size;
			this.mallocBtn.disabled = false;
			this.console.value += 'set memory size to [' + size + ']\n';
			this.pool = new MSlice(this.uuid++, size, false);
			this.avail = size;
			this.draw();
		}
		else {
			alert('memory size invalid');
		}
	}
	this.malloc = function() {
		var size = parseInt(document.getElementById('mRequest').value);
		if (size > this.avail) {
			alert('not enough memory');
		}
		else if (size > 0) {
			var p = this.pool;
			while (p) {
				if(p.used == false && size <= p.size) {
					if (size == p.size)
						p.used = true;
					else {
						var tmp = new MSlice(this.uuid++, size, true);
						p.size -= size;
						tmp.prev = p.prev;
						tmp.next = p;
						if (p.prev == null)
							this.pool = tmp;
						else
							p.prev.next = tmp;
						p.prev = tmp;
					}
					this.avail -= size;
					if (this.avail == 0)
						this.mallocBtn.disabled = true;
					this.used += size;
					this.freeBtn.disabled = false;
					break;
				}
				else
					p = p.next;
			}
			if (p == null)
				alert('没有足够大的空闲内存块');
			this.draw();
		}
		else {
			alert('请求的内存大小无效');
		}
	}
	this.free = function() {
		var uuid = parseInt(document.getElementById('mHandle').value);
		if (uuid >= 0) {
			var p = this.pool;
			while (p) {
				if (uuid == p.uuid) {
					if (p.used) {
						p.used = false;
						this.avail += p.size;
						this.used -= p.size;
						if (this.used == 0)
							this.freeBtn.disabled = true;
						if (p.pref == null && p.next == null) {
						}
						else if (p.prev == null) {
							if (p.next.used == false) {
								p.next.size += p.size;
								p.next.prev = null;
								this.pool = p.next;
								delete p;
							}
						}
						else if (p.next == null) {
							if (p.prev.used == false) {
								p.prev.size += p.size;
								p.prev.next = null;
								delete p;
							}
						}
						else {
							if (p.prev.used == false && p.next.used == false) {
								p.prev.size += p.size + p.next.size;
								p.prev.next = p.next.next;
								if (p.next.next)
									p.next.next.prev = p.prev;
								delete p.next;
								delete p;
							}
							else if (p.prev.used == false) {
								p.prev.size += p.size;
								p.prev.next = p.next;
								p.next.prev = p.prev;
								delete p;
							}
							else if (p.next.used == false) {
								p.next.size += p.size;
								p.prev.next = p.next;
								p.next.prev = p.prev;
								delete p;
							}
							else {
							}
						}
						if (this.avail > 0)
							this.mallocBtn.disabled = false;
					}
					else
						alert('指定的内存块未被使用');
					break;
				}
				else
					p = p.next;
			}
			if (p == null)
				alert('不存在的内存句柄');
		}
		else
			alert('无效的内存句柄');
		this.draw();
	}
	this.draw = function() {
		var ctx = this.table.getContext('2d');
		var left = this.table.getBoundingClientRect().left;
		var top = this.table.getBoundingClientRect().top;
		var right = this.table.getBoundingClientRect().right;
		var bottom = this.table.getBoundingClientRect().bottom;
		var width = right - left;
		var ratio = (bottom - top) / this.size;
		var p = this.pool;
		var y = 0;
		ctx.textBaseline = "middle";
		ctx.textAlign = "center";
		var color = true;
		while (p) {
			if (p.used) {
				if (color) {
					ctx.fillStyle = '#FF0000';
					color = false;
				}
				else {
					ctx.fillStyle = '#FFA500';
					color = true;
				}
			}
			else
				ctx.fillStyle= '#008000';
			ctx.fillRect(0, y, width, p.size*ratio);
			ctx.fillStyle = '#FFFFFF';
			ctx.fillText(p.uuid + '(size=' + p.size + ')', width/2, y + (p.size * ratio)/2);
			y += p.size*ratio;
			p = p.next;
		}
	}
}
