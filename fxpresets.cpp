#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QFileDialog>
#include <QInputDialog>

#include "fxpresets.h"
#include "utils.h"

CFxPresets::CFxPresets (QWidget *parent): QWidget (parent)
{
  QHBoxLayout *h_box = new QHBoxLayout;
  setLayout (h_box);
  
  cmb_presets = new QComboBox;
  QPushButton *bt_menu = new QPushButton (tr ("Menu"));

  h_box->addWidget (cmb_presets);
  h_box->addWidget (bt_menu);
  
  connect (cmb_presets, SIGNAL(currentIndexChanged (const QString &)),
           this, SLOT(cmb_presets_currentIndexChanged (const QString &)));

  QMenu *menu = new QMenu;
  bt_menu->setMenu (menu);
  
  menu_add_item (this, menu, tr ("Bank new"), SLOT (bank_new_click()), "", "");
  menu_add_item (this, menu, tr ("Bank load"), SLOT (bank_load_click()), "", "");
  menu_add_item (this, menu, tr ("Bank save"), SLOT (bank_save_click()), "", "");
  menu_add_item (this, menu, tr ("Bank save as"), SLOT (bank_save_as_click()), "", "");

  menu_add_item (this, menu, tr ("Preset save"), SLOT (preset_save()), "", "");
  menu_add_item (this, menu, tr ("Preset save as"), SLOT (preset_save_as()), "", "");
  
}


void CFxPresets::load_bank_file (const QString &fname)
{
  if (fname.isNull() || fname.isEmpty())
     return;

  cmb_presets->clear();
  map.clear();
  map = map_load_keyval (fname);
  
  cmb_presets->addItems (map.keys());
}


void CFxPresets::save_bank_file (const QString &fname)
{
  if (fname.startsWith (":"))
     return;
     
  QString s = map_keyval_to_string (map);
  qstring_save (fname, s);
}


void CFxPresets::cmb_presets_currentIndexChanged (const QString &text)
{
  emit preset_changed (map[text]);
}


void CFxPresets::bank_new_click()
{
  path_bank = "";
  map.clear();

}
 

void CFxPresets::bank_save_click()
{


}


void CFxPresets::bank_save_as_click()
{
  QString f = QFileDialog::getSaveFileName (this, tr ("Save File"), banks_path);
  if (f.isNull())
     return;
  
  path_bank = f;

}


void CFxPresets::bank_load_click()
{
  QString f = QFileDialog::getOpenFileName (this, tr ("Open File"), banks_path);  
  if (f.isNull())
     return;
  
  path_bank = f;

  save_bank_file (path_bank);
}


void CFxPresets::update_presets()
{
  
}


void CFxPresets::preset_save_as()
{
  bool ok;
  QString text = QInputDialog::getText(this, tr("Save preset as"),
                                         tr("Name:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
        {
         emit save_request();
         map[text] = preset_data;
         cmb_presets->addItem (text);
        }
}
 

void CFxPresets::preset_save()
{
  emit save_request();
  
  map[cmb_presets->currentText()] = preset_data;

}
