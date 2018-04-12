<?php

/**
 * @author Semyon Dragunov <sam.dragunov@gmail.com>
 * https://github.com/SemyonDragunov
 */

/**
 * UI controller for sl7_gallery_album.
 */
class SL7GalleryAlbumUIController extends EntityDefaultUIController {

  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%sl7_gallery_album';

    $items[$this->path . '/view'] = array(
      'title' => 'Создать альбом',
      'description' => 'Создать альбом.',
      'page callback'    => 'sl7_gallery_album_get_edit_form',
      'page arguments'   => array('create', NULL),
      'access callback'  => 'sl7_gallery_album_access',
      'access arguments' => array('create'),
      'type'   => MENU_LOCAL_TASK,
      'context'=> MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file'             => 'sl7_gallery_album.ui.inc',
      'file path'        => drupal_get_path('module', 'sl7_gallery') . '/includes',
    );

    $items[$this->path . '/view'] = array(
      'title'  => 'Просмотр списка альбомов',
      'type'   => MENU_DEFAULT_LOCAL_TASK,
      'context'=> MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => -10,
    );

    $items[$this->path . '/add'] = array(
      'title'            => 'Создать альбом',
      'page callback'    => 'sl7_gallery_album_get_edit_form',
      'page arguments'   => array('create', NULL),
      'access callback'  => 'sl7_gallery_album_access',
      'access arguments' => array('create'),
      'type'             => MENU_LOCAL_TASK,
      'weight'           => -9,
      'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/' . $wildcard] = array(
      'title callback'   => 'sl7_gallery_album_title',
      'title arguments'  => array($id_count),
      'page callback'    => 'entity_ui_entity_page_view',
      'page arguments'   => array($id_count),
      'access callback'  => 'sl7_gallery_album_access',
      'access arguments' => array('view', $id_count),
      'load arguments'   => array($this->entityType),
      'type'             => MENU_CALLBACK,
    );

    $items[$this->path . '/' . $wildcard . '/view'] = array(
      'title'  => 'Просмотр альбома',
      'type'   => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );

    $items[$this->path . '/' . $wildcard . '/edit'] = array(
      'title'            => 'Редактировать альбом',
      'page callback'    => 'sl7_gallery_album_get_edit_form',
      'page arguments'   => array('edit', $id_count),
      'load arguments'   => array($this->entityType),
      'access callback'  => 'sl7_gallery_album_access',
      'access arguments' => array('edit', $id_count),
      'type'             => MENU_LOCAL_TASK,
      'weight'           => -8,
      'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/' . $wildcard . '/delete'] = array(
      'title'            => 'Удалить альбом',
      'page callback'    => 'drupal_get_form',
      'page arguments'   => array('sl7_gallery_album_delete_form', $id_count),
      'load arguments'   => array($this->entityType),
      'access callback'  => 'sl7_gallery_album_access',
      'access arguments' => array('delete', $id_count),
      'type'             => MENU_LOCAL_TASK,
      'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight'           => -7,
    );

    $module_path = drupal_get_path('module', $this->entityInfo['module']);
    foreach ($items as &$item) {
      $item['file'] = 'includes/sl7_gallery_album.ui.inc';
      $item['file path'] = $module_path;
    }
    unset($item);

    return $items;
  }

  public function form($form, &$form_state) {

    $album = $form_state['sl7_gallery_album'];

    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => 'Название альбома',
      '#default_value' => !empty($album->label) ? $album->label : '',
      '#maxlength' => 128,
      '#required' => TRUE,
    );

    field_attach_form('sl7_gallery_album', $album, $form, $form_state);

    $form['actions'] = array(
      '#type' => 'actions',
    );
    $form['actions']['submit'] = array(
      '#type'   => 'submit',
      '#value'  => !empty($album->aid) ? 'Сохранить' : 'Создать',
      '#weight' => 0,
    );
    $form['actions']['delete'] = array(
      '#markup' => !empty($album->aid) ? l(t('Delete'), $album->realPath() . '/delete', array('attributes' => array('class' => array('button remove')), 'query' => array('destination' => SL7_GALLERY_ALBUM_PATH))) : '',
    );

    return $form;
  }

  public function formValidate($form, &$form_state) {

  }

  public function formSubmit($form, &$form_state) {
    $album = $this->entityFormSubmitBuildEntity($form, $form_state);
    $album->save();
    $label = check_plain($album->label);

    $form_state['redirect'] = $album->realPath();

    if (!empty($album->is_new)) {
      drupal_set_message("Альбом \"$label\" создан.");
    }
    else {
      drupal_set_message("Альбом \"$label\" сохранен.");
    }
  }

  public function deleteForm($form, &$form_state) {
    $album = $form_state['sl7_gallery_album'];
    $label = check_plain($album->label);

    $form = confirm_form($form,
      "Вы уверены в удалении альбома \"$label\"?",
      $album->realPath(),
      '<p>' . 'Удаление не может быть отменено.' . '</p>',
      t('Delete'),
      t('Cancel'),
      'confirm'
    );

    return $form;
  }

  public function deleteFormSubmit($form, &$form_state) {
    $album = $form_state['sl7_gallery_album'];
    $label = check_plain($album->label);

    sl7_gallery_album_delete($album);

    drupal_set_message("Альбом \"$label\" удалено.");

    $form_state['redirect'] = SL7_GALLERY_ALBUM_PATH;
  }
}

function sl7_gallery_album_edit_form($form, &$form_state) {
  return entity_ui_controller($form_state['entity_type'])->form($form, $form_state);
}

function sl7_gallery_album_edit_form_validate($form, &$form_state) {
  entity_ui_controller($form_state['entity_type'])->formValidate($form, $form_state);
}

function sl7_gallery_album_edit_form_submit($form, &$form_state) {
  entity_ui_controller($form_state['entity_type'])->formSubmit($form, $form_state);
}

function sl7_gallery_album_delete_form($form, &$form_state, $album) {
  $entity_type = 'sl7_gallery_album';
  $form_state['entity_type'] = $entity_type;
  $form_state[$entity_type] = $album;
  return entity_ui_controller($form_state['entity_type'])->deleteForm($form, $form_state);
}

function sl7_gallery_album_delete_form_submit($form, &$form_state) {
  entity_ui_controller($form_state['entity_type'])->deleteFormSubmit($form, $form_state);
}

function sl7_gallery_album_get_edit_form($op = 'create', $album = NULL) {
  $form_state = array(
    'entity_type' => 'sl7_gallery_album',
    'op' => $op,
  );

  switch ($op) {
    case 'create':
      drupal_set_title('Создать альбом');
      $form_state['sl7_gallery_album'] = sl7_gallery_album_create();
      break;
    case 'edit':
      $form_state['sl7_gallery_album'] = is_object($album) ? $album : sl7_gallery_album_load($album);
      drupal_set_title('Редактирование ' . $form_state['sl7_gallery_album']->label);
      break;
    default:
      $form_state['sl7_gallery_album'] = $album;
      break;
  }

  form_load_include($form_state, 'inc', 'sl7_gallery_album', 'includes/sl7_gallery_album.ui');

  $form_state['build_info']['args'] = array($op, $album);

  return drupal_build_form('sl7_gallery_album_edit_form', $form_state);
}