/*
* Copyright (c) 1998, Yun Xinyi <yunxinyi@gmail.com>
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Yun Xinyi, <yunxinyi@gmail.com> nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY YUN XINYI AND CONTRIBUTORS "AS IS" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL YUN XINYI AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
* FileName	: DiskManager.java
* Description	: 一个位视图磁盘空间管理的demo
* Last-Modified: Wed May  8 08:47:44 CST 2013
* Author	: 张盼
*/


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.text.*;
import java.text.*;
import java.util.regex.*;

abstract class RegexDocument extends PlainDocument {
	private String regex = null;
	private Pattern pattern = null;

	public RegexDocument() {
		super();
	}

	public RegexDocument(String regex) {
		super();
		this.setRegex(regex);
	}

	public String getRegex() {
		return regex;
	}

	public void setRegex(String regex) throws PatternSyntaxException {
		this.regex = regex;
		this.pattern = Pattern.compile(regex);
	}

	public void insertString(int offs, String str, AttributeSet a) throws
	BadLocationException {
		String oldstr = this.getText(0, this.getLength());
		StringBuffer sb = new StringBuffer(oldstr);
		sb.insert(offs, str);
		String curstr = sb.toString();
//		System.out.println(curstr);

		if (pattern != null) {
			if (pattern.matcher(curstr).matches()) {
				super.insertString(offs, str, a);
			}
		}
		else {
			Toolkit.getDefaultToolkit().beep();
		}
	}
}

public class DiskManager extends JFrame {
	private int Cylinder;
	private int Track;
	private int Sector;
	private int Count;
	private int Used;
	private int[] sectorArray;

	private Container mainPane;
	private DMChartPanel chartPanel;
	private JPanel controlPanel;
	private JTextField CylinderTF;
	private JTextField TrackTF;
	private JTextField SectorTF;
	private JLabel warnLabel;
	private JButton initButton;
	private JButton requestButton;
	private JButton releaseButton;
	private JButton helpButton;
	private JButton quitButton;

	private void help() {
		JOptionPane.showMessageDialog(this, "<html><b>帮助:</b><br /><ol><li>要开始，请先设置磁盘空间</li><li>请求磁盘空间会在左边的位视图中显示分配的位，并在<br />右上角对应的的文本框中显示出柱面，磁道和扇区号</li><li>释放磁盘空间需要在文本框中输入要释放的空间的对应<br />信息，如果输入正确，则对应的扇区会被释放</li></ol></html>", "帮助信息", JOptionPane.INFORMATION_MESSAGE);
	}

	private void initDisk() {
		String tmp = CylinderTF.getText().trim();
		boolean ok = true;
		if (tmp.equals("")) {
			warnLabel.setText("<html><center color=red>柱面不能为空,请重试</center></html>");
			ok = false;
		}
		if (ok && ((Cylinder = Integer.parseInt(tmp)) == 0)) {
			warnLabel.setText("<html><center color=red>柱面不能为0,请重试</center></html>");
			ok = false;
		}
		if (ok && ((tmp = TrackTF.getText().trim()).equals(""))) {
			warnLabel.setText("<html><center color=red>磁道不能为空,请重试</center></html>");
			ok = false;
		}
		if (ok && ((Track = Integer.parseInt(tmp))) == 0) {
			warnLabel.setText("<html><center color=red>磁道不能为0,请重试</center></html>");
			ok = false;
		}
		if (ok && ((tmp = SectorTF.getText().trim()).equals(""))) {
			warnLabel.setText("<html><center color=red>扇区不能为空,请重试</center></html>");
			ok = false;
		}
		if (ok && ((Sector = Integer.parseInt(tmp)) == 0)) {
			warnLabel.setText("<html><center color=red>扇区不能为0,请重试</center></html>");
			ok = false;
		}
		if (ok) {
			warnLabel.setText("<html><center color=green>磁盘设置成功</center></html>");
		}
		else {
			requestButton.setEnabled(false);
			releaseButton.setEnabled(false);
			return;
		}
		if (Cylinder == 0 || Track == 0 || Sector == 0) {
			requestButton.setEnabled(false);
			String warnMsg = "<html><center color=red>未知错误</center></html>";
			JOptionPane.showMessageDialog(this, warnMsg, "初始化磁盘", JOptionPane.ERROR_MESSAGE);
		}
		else {
			requestButton.setEnabled(true);
			Count = Cylinder * Track * Sector;
			Used = 0;
			sectorArray = new int[Count];
			repaint();
		}
		releaseButton.setEnabled(false);
	}

	private void request() {
		int i;
		for (i = 0; i < Count; ++i) {
			if (sectorArray[i] == 0)
				break;
		}
		sectorArray[i] = 1;
		CylinderTF.setText(""+i/8);
		TrackTF.setText(""+(i%8)/Sector);
		SectorTF.setText(""+i%Sector);
		++Used;
		if (Used == Count)
			requestButton.setEnabled(false);
		releaseButton.setEnabled(true);
		repaint();
	}

	private void release() {
		int C = Integer.parseInt(CylinderTF.getText());
		int T = Integer.parseInt(TrackTF.getText());
		int S = Integer.parseInt(SectorTF.getText());
		int index = C*8 + T*Sector + S;
		if (index > Count) {
			JOptionPane.showMessageDialog(this, "<html><b color=red>非法的参数值，请输入正确的参数</b></html>", "释放磁盘空间", JOptionPane.WARNING_MESSAGE);
		}
		else if (sectorArray[index] == 0) {
			JOptionPane.showMessageDialog(this, "<html><b color=red>指定的磁盘空间已经被释放或还未被占用</b></html>", "释放磁盘空间", JOptionPane.WARNING_MESSAGE);
		}
		else {
			sectorArray[index] = 0;
			--Used;
			if (Used == 0)
				releaseButton.setEnabled(false);
			requestButton.setEnabled(true);
			repaint();
		}
	}
	
	public DiskManager() {
		mainPane = getContentPane();

		setTitle("磁盘管理(位视图)");
		setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		setSize(600, 400);
		setLocationRelativeTo(null);

		chartPanel = new DMChartPanel();
		controlPanel = new JPanel();
		controlPanel.setBorder(new TitledBorder("操作:"));
		GridBagLayout gridBagLayout = new GridBagLayout();
		controlPanel.setLayout(gridBagLayout);
		GridBagConstraints gbc = new GridBagConstraints();

		JLabel CylinderLabel = new JLabel("柱面:");
		JLabel TrackLabel = new JLabel("磁道:");
		JLabel SectorLabel = new JLabel("扇区:");
		warnLabel = new JLabel("<html><center color=green>要开始,请点击设置磁盘</center></html>");
		CylinderTF = new JTextField(2);
		TrackTF = new JTextField(2);
		SectorTF = new JTextField(2);
		RegexDocument CregexDoc = new RegexDocument("^[0-9]{1,2}$"){};
		RegexDocument TregexDoc = new RegexDocument("^[0-9]{1,2}$"){};
		RegexDocument SregexDoc = new RegexDocument("^[0-9]{1,2}$"){};
		CylinderTF.setDocument(CregexDoc);
		TrackTF.setDocument(TregexDoc);
		SectorTF.setDocument(SregexDoc);
		CylinderTF.setText("8");
		TrackTF.setText("4");
		SectorTF.setText("2");
		initButton = new JButton("设置磁盘空间");
		initButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				initDisk();
			}
		});
		requestButton = new JButton("请求磁盘空间");
		requestButton.setEnabled(false);
		requestButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				request();
			}
		});
		releaseButton = new JButton("释放磁盘空间");
		releaseButton.setEnabled(false);
		releaseButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				release();
			}
		});
		helpButton = new JButton("帮助");
		helpButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				help();
			}
		});
		quitButton = new JButton("退出");
		quitButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});

		gbc.fill = GridBagConstraints.BOTH;
		gbc.gridwidth = 1;
		gbc.gridheight = 1;
		gridBagLayout.setConstraints(CylinderLabel, gbc);
		controlPanel.add(CylinderLabel, gbc);
		gbc.gridx = GridBagConstraints.RELATIVE;
		gridBagLayout.setConstraints(CylinderTF, gbc);
		controlPanel.add(CylinderTF, gbc);
		gbc.gridx = 0;
		gbc.gridy = GridBagConstraints.RELATIVE;
		gridBagLayout.setConstraints(TrackLabel, gbc);
		controlPanel.add(TrackLabel, gbc);
		gbc.gridx = GridBagConstraints.RELATIVE;
		gbc.gridy = 1;
		gridBagLayout.setConstraints(TrackTF, gbc);
		controlPanel.add(TrackTF, gbc);
		gbc.gridx = 0;
		gbc.gridy = GridBagConstraints.RELATIVE;
		gridBagLayout.setConstraints(SectorLabel, gbc);
		controlPanel.add(SectorLabel, gbc);
		gbc.gridx = GridBagConstraints.RELATIVE;
		gbc.gridy = 2;
		gridBagLayout.setConstraints(SectorTF, gbc);
		controlPanel.add(SectorTF, gbc);
		gbc.gridx = 0;
		gbc.gridy = 3;
		gbc.gridwidth = 2;
		gridBagLayout.setConstraints(warnLabel, gbc);
		controlPanel.add(warnLabel, gbc);
		gbc.gridx = 0;
		gbc.gridy = 4;
		gridBagLayout.setConstraints(initButton, gbc);
		controlPanel.add(initButton, gbc);
		gbc.gridy = 5;
		gridBagLayout.setConstraints(requestButton, gbc);
		controlPanel.add(requestButton, gbc);
		gbc.gridy = 6;
		gridBagLayout.setConstraints(releaseButton, gbc);
		controlPanel.add(releaseButton, gbc);
		gbc.gridy = 7;
		JButton infoButton = new JButton("<html><table rules=all><caption>位图说明</caption><tr><td>未初始化:</td><td bgcolor=yellow>&nbsp;&nbsp;&nbsp;&nbsp;</td></tr><tr><td>未分配:</td><td bgcolor=green>&nbsp;&nbsp;&nbsp;&nbsp;</td></tr><hr /><tr><td>已分配:</td><td bgcolor=red>&nbsp;&nbsp;&nbsp;&nbsp;</td></tr></table></html>");
		infoButton.setEnabled(false);
		gridBagLayout.setConstraints(infoButton, gbc);
		controlPanel.add(infoButton, gbc);
gbc.anchor = GridBagConstraints.BASELINE;
		gbc.gridy = 8;
		gridBagLayout.setConstraints(helpButton, gbc);
		controlPanel.add(helpButton, gbc);
		gbc.gridy = 9;
		gridBagLayout.setConstraints(quitButton, gbc);
		controlPanel.add(quitButton, gbc);

		mainPane.add(chartPanel, BorderLayout.CENTER);
		mainPane.add(controlPanel, BorderLayout.EAST);
		setVisible(true);
	}

	private class DMChartPanel extends JPanel {
		public DMChartPanel() {
			super();
			setBorder(new TitledBorder("磁盘空间位视图:"));
		}

		public void paintComponent(Graphics gs) {
			int xlen = (int)Math.ceil(Math.sqrt(Count));
			int ylen = xlen;
			Dimension size = getSize();
			Graphics2D g = (Graphics2D) gs;
			int x = 8;
			int y = 24;
			int w = size.width - 18;
			int h = size.height - 34;
			if (xlen == 0) {
				g.setColor(Color.YELLOW);
				g.fillRect(x, y, w, h);
				return;
			}
			int xw;
			int yh;
			if (xlen * (ylen-1) > Count) {
				--ylen;
				xw = (w-xlen-1)/xlen;
				yh = (h-ylen-1)/ylen;
			}
			else {
				xw = (w-xlen-1)/xlen;
				yh = (h-ylen-1)/ylen;
			}
			for (int i = 0; i < Count; ++i) {
				if (sectorArray[i] == 0)
					g.setColor(Color.GREEN);
				else
					g.setColor(Color.RED);
				if (i != 0 && i % xlen == 0) {
					y += yh + 1;
					x = 8;
				}
				g.fillRect(x, y, xw, yh);
				x += xw + 1;
			}
		}
	}

	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}
		catch (Exception e) {
		}
		DiskManager dm = new DiskManager();
	}
}
