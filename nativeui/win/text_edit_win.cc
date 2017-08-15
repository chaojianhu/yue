// Copyright 2017 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "nativeui/text_edit.h"

#include "base/strings/utf_string_conversions.h"
#include "nativeui/win/edit_view.h"

namespace nu {

TextEdit::TextEdit() {
  auto* edit = new EditView(this, WS_VSCROLL | WS_HSCROLL | ES_MULTILINE);
  edit->set_switch_focus_on_tab(false);
  edit->SetPlainText();
  TakeOverView(edit);
}

TextEdit::~TextEdit() {
}

void TextEdit::SetText(const std::string& text) {
  static_cast<EditView*>(GetNative())->SetText(text);
}

std::string TextEdit::GetText() const {
  return static_cast<EditView*>(GetNative())->GetText();
}

void TextEdit::Redo() {
  static_cast<EditView*>(GetNative())->Redo();
}

bool TextEdit::CanRedo() const {
  return static_cast<EditView*>(GetNative())->CanRedo();
}

void TextEdit::Undo() {
  static_cast<EditView*>(GetNative())->Undo();
}

bool TextEdit::CanUndo() const {
  return static_cast<EditView*>(GetNative())->CanUndo();
}

void TextEdit::Cut() {
  static_cast<EditView*>(GetNative())->Cut();
}

void TextEdit::Copy() {
  static_cast<EditView*>(GetNative())->Copy();
}

void TextEdit::Paste() {
  static_cast<EditView*>(GetNative())->Paste();
}

void TextEdit::Clear() {
  static_cast<EditView*>(GetNative())->Clear();
}

std::tuple<int, int> TextEdit::GetSelectionRange() const {
  HWND hwnd = static_cast<SubwinView*>(GetNative())->hwnd();
  int start, end;
  ::SendMessage(hwnd, EM_GETSEL, reinterpret_cast<WPARAM>(&start),
                                 reinterpret_cast<LPARAM>(&end));
  if (start == end)
    return std::make_tuple(-1, -1);
  else
    return std::make_tuple(start, end);
}

void TextEdit::SelectRange(int start, int end) {
  HWND hwnd = static_cast<SubwinView*>(GetNative())->hwnd();
  ::SendMessage(hwnd, EM_SETSEL, start, end);
  ::SendMessage(hwnd, EM_SCROLLCARET, 0, 0L);
}

std::string TextEdit::GetTextInRange(int start, int end) const {
  return GetText().substr(start, end - start);
}

void TextEdit::InsertText(const std::string& text) {
  HWND hwnd = static_cast<SubwinView*>(GetNative())->hwnd();
  ::SendMessageW(hwnd, EM_REPLACESEL, FALSE,
                 reinterpret_cast<LPARAM>(base::UTF8ToUTF16(text).c_str()));
}

void TextEdit::InsertTextAt(const std::string& text, int pos) {
  SelectRange(pos, pos);
  InsertText(text);
}

void TextEdit::Delete() {
  HWND hwnd = static_cast<SubwinView*>(GetNative())->hwnd();
  ::SendMessage(hwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(""));
}

void TextEdit::DeleteRange(int start, int end) {
  SelectRange(start, end);
  InsertText("");
}

}  // namespace nu