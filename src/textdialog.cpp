/***************************************************************************
 *   Copyright (C) 2006 by Massimiliano Torromeo                           *
 *   massimiliano.torromeo@gmail.com                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "textdialog.h"

#include "gmenu2x.h"
#include "utilities.h"

using namespace std;

TextDialog::TextDialog(GMenu2X *gmenu2x, const string &title, const string &description, const string &icon, vector<string> *text)
	: Dialog(gmenu2x)
{
	this->text = text;
	this->title = title;
	this->description = description;
	this->icon = icon;
	preProcess();
}

void TextDialog::preProcess() {
	unsigned i = 0;
	string row;

	while (i<text->size()) {
		//clean this row
		row = trim(text->at(i));

		//check if this row is not too long
		if (gmenu2x->font->getTextWidth(row)>(int)gmenu2x->resX-15) {
			vector<string> words;
			split(words, row, " ");

			unsigned numWords = words.size();
			//find the maximum number of rows that can be printed on screen
			while (gmenu2x->font->getTextWidth(row)>(int)gmenu2x->resX-15 && numWords>0) {
				numWords--;
				row = "";
				for (unsigned x=0; x<numWords; x++)
					row += words[x] + " ";
				row = trim(row);
			}

			//if numWords==0 then the string must be printed as-is, it cannot be split
			if (numWords>0) {
				//replace with the shorter version
				text->at(i) = row;

				//build the remaining text in another row
				row = "";
				for (unsigned x=numWords; x<words.size(); x++)
					row += words[x] + " ";
				row = trim(row);

				if (!row.empty())
					text->insert(text->begin()+i+1, row);
			}
		}
		i++;
	}
}

void TextDialog::drawText(vector<string> *text, unsigned int y,
		unsigned int firstRow, unsigned int rowsPerPage)
{
	const int fontHeight = gmenu2x->font->getLineSpacing();

	for (unsigned i = firstRow; i < firstRow + rowsPerPage && i < text->size(); i++) {
		const string &line = text->at(i);
		int rowY = y + (i - firstRow) * fontHeight;
		if (line == "----") { // horizontal ruler
			rowY += fontHeight / 2;
			gmenu2x->s->hline(5, rowY, gmenu2x->resX - 16, 255, 255, 255, 130);
			gmenu2x->s->hline(5, rowY+1, gmenu2x->resX - 16, 0, 0, 0, 130);
		} else {
			gmenu2x->font->write(gmenu2x->s, line, 5, rowY);
		}
	}

	gmenu2x->drawScrollBar(rowsPerPage, text->size(), firstRow);
}

void TextDialog::exec() {
	bool close = false;

	Surface bg(gmenu2x->bg);

	//link icon
	if (!fileExists(icon))
		drawTitleIcon("icons/ebook.png",true,&bg);
	else
		drawTitleIcon(icon,false,&bg);
	writeTitle(title,&bg);
	writeSubTitle(description,&bg);

	gmenu2x->drawButton(&bg, "start", gmenu2x->tr["Exit"],
	gmenu2x->drawButton(&bg, "cancel", "",
	gmenu2x->drawButton(&bg, "down", gmenu2x->tr["Scroll"],
	gmenu2x->drawButton(&bg, "up", "", 5)-10))-10);

	bg.convertToDisplayFormat();

	const int fontHeight = gmenu2x->font->getLineSpacing();
	unsigned int contentY, contentHeight;
	tie(contentY, contentHeight) = gmenu2x->getContentArea();
	const unsigned rowsPerPage = contentHeight / fontHeight;
	contentY += (contentHeight % fontHeight) / 2;

	unsigned firstRow = 0;
	while (!close) {
		bg.blit(gmenu2x->s, 0, 0);
		drawText(text, contentY, firstRow, rowsPerPage);
		gmenu2x->s->flip();

		switch(gmenu2x->input.waitForPressedButton()) {
			case InputManager::UP:
				if (firstRow > 0) firstRow--;
				break;
			case InputManager::DOWN:
				if (firstRow + rowsPerPage < text->size()) firstRow++;
				break;
			case InputManager::ALTLEFT:
				if (firstRow >= rowsPerPage-1) firstRow -= rowsPerPage-1;
				else firstRow = 0;
				break;
			case InputManager::ALTRIGHT:
				if (firstRow + rowsPerPage*2 -1 < text->size()) {
					firstRow += rowsPerPage-1;
				} else {
					firstRow = text->size() < rowsPerPage ?
						0 : text->size() - rowsPerPage;
				}
				break;
			case InputManager::SETTINGS:
			case InputManager::CANCEL:
				close = true;
				break;
			default:
				break;
		}
	}
}
