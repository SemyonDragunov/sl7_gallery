<?php

/**
 * @author Semyon Dragunov <sam.dragunov@gmail.com>
 * https://github.com/SemyonDragunov
 */

/**
 * UI controller for sl7_gallery_image.
 */
class SL7GalleryImageUIController extends EntityDefaultUIController {

  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%sl7_gallery_image';

    // Ссылка для альбома
    $items[SL7_GALLERY_ALBUM_PATH . '/%/add'] = array(
      'title'            => 'Добавить изображение',
      'page callback'    => 'sl7_gallery_image_get_edit_form',
      'page arguments'   => array('create', 1, NULL),
      'access callback'  => 'sl7_gallery_image_access',
      'access arguments' => array('create'),
      'weight'           => -9,
      'type'             => MENU_LOCAL_TASK,
      'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/' . $wildcard] = array(
      'title callback'   => 'sl7_gallery_image_title',
      'title arguments'  => array($id_count),
      'page callback'    => 'entity_ui_entity_page_view',
      'page arguments'   => array($id_count),
      'access callback'  => 'sl7_gallery_image_access',
      'access arguments' => array('view', $id_count),
      'load arguments'   => array($this->entityType),
      'type'             => MENU_CALLBACK,
    );

    $items[$this->path . '/' . $wildcard . '/edit'] = array(
      'title'            => t('Edit'),
      'page callback'    => 'sl7_gallery_image_get_edit_form',
      'page arguments'   => array('edit', NULL, $id_count),
      'load arguments'   => array($this->entityType),
      'access callback'  => 'sl7_gallery_image_access',
      'access arguments' => array('edit', $id_count),
      'type'             => MENU_LOCAL_TASK,
      'weight'           => -9,
      'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/' . $wildcard . '/delete'] = array(
      'title'            => t('Delete'),
      'page callback'    => 'drupal_get_form',
      'page arguments'   => array('sl7_gallery_image_delete_form', $id_count),
      'load arguments'   => array($this->entityType),
      'access callback'  => 'sl7_gallery_image_access',
      'access arguments' => array('delete', $id_count),
      'type'             => MENU_LOCAL_TASK,
      'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight'           => -7,
    );

    $module_path = drupal_get_path('module', $this->entityInfo['module']);
    foreach ($items as &$item) {
      $item['file'] = 'includes/sl7_gallery_image.ui.inc';
      $item['file path'] = $module_path;
    }
    unset($item);

    return $items;
  }

  public function form($form, &$form_state) {

    $image = $form_state['sl7_gallery_image'];

    if (!$image->aid) {
      drupal_set_message('Ошибка назначения альбома.' , 'error');
      return $form;
    }

    field_attach_form('sl7_gallery_image', $image, $form, $form_state);

    $form['cover'] = array(
      '#type' => 'checkbox',
      '#title' => 'Обложка альбома',
      '#description' => 'Сделать это изображение обложкой альбома.',
      '#default_value' => 0,
    );

    $form['actions'] = array(
      '#type' => 'actions',
    );
    $form['actions']['submit'] = array(
      '#type'   => 'submit',
      '#value'  => !empty($image->iid) ? 'Сохранить' : 'Добавить',
      '#weight' => 0,
    );
    $form['actions']['more'] = array(
      '#type'   => 'submit',
      '#value'  => 'Сохранить и добавить ещё',
      '#weight' => 1,
    );
    $form['actions']['delete'] = array(
      '#markup' => !empty($image->iid) ? l(t('Delete'), $image->realPath() . '/delete', array('attributes' => array('class' => array('button remove')), 'query' => array('destination' => SL7_GALLERY_ALBUM_PATH . '/' . $image->aid))) : '',
      '#weight' => 2,
    );

    return $form;
  }

  public function formValidate($form, &$form_state) {

  }

  public function formSubmit($form, &$form_state) {
    $image = $this->entityFormSubmitBuildEntity($form, $form_state);
    if ($form_state['values']['cover'] == TRUE) {
      sl7_gallery_album_set_cover($image);
    }
    $image->save();

    if (!empty($image->is_new)) {
      drupal_set_message("Изображение создано.");
    }
    else {
      drupal_set_message("Изображение сохранено.");
    }

    if ($form_state['triggering_element']['#array_parents'][1] == 'more') {
      $form_state['redirect'] = SL7_GALLERY_ALBUM_PATH . '/' . $image->aid . '/add';
    }
    else {
      $form_state['redirect'] = SL7_GALLERY_ALBUM_PATH . '/' . $image->aid;
    }
  }

  public function deleteForm($form, &$form_state) {
    $image = $form_state['sl7_gallery_image'];

    $form = confirm_form($form,
      "Вы уверены в удалении изображения?",
      $image->realPath(),
      '<p>' . 'Удаление не может быть отменено.' . '</p>',
      t('Delete'),
      t('Cancel'),
      'confirm'
    );

    return $form;
  }

  public function deleteFormSubmit($form, &$form_state) {
    $image = $form_state['sl7_gallery_image'];

    sl7_gallery_image_delete($image);

    drupal_set_message("Изображение удалено.");

    $form_state['redirect'] = SL7_GALLERY_ALBUM_PATH . '/' . $image->aid;
  }
}

function sl7_gallery_image_edit_form($form, &$form_state) {
  return entity_ui_controller($form_state['entity_type'])->form($form, $form_state);
}

function sl7_gallery_image_edit_form_validate($form, &$form_state) {
  entity_ui_controller($form_state['entity_type'])->formValidate($form, $form_state);
}

function sl7_gallery_image_edit_form_submit($form, &$form_state) {
  entity_ui_controller($form_state['entity_type'])->formSubmit($form, $form_state);
}

function sl7_gallery_image_delete_form($form, &$form_state, $image) {
  $entity_type = 'sl7_gallery_image';
  $form_state['entity_type'] = $entity_type;
  $form_state[$entity_type] = $image;
  return entity_ui_controller($form_state['entity_type'])->deleteForm($form, $form_state);
}

function sl7_gallery_image_delete_form_submit($form, &$form_state) {
  entity_ui_controller($form_state['entity_type'])->deleteFormSubmit($form, $form_state);
}

function sl7_gallery_image_get_edit_form($op = 'create', $aid = NULL, $image = NULL) {
  $form_state = array(
    'entity_type' => 'sl7_gallery_image',
    'op' => $op,
  );

  switch ($op) {
    case 'create':
      $album = sl7_gallery_album_load($aid);
      drupal_set_title('Добавить изображение для альбома ' . $album->label);
      $form_state['sl7_gallery_image'] = sl7_gallery_image_create();
      $form_state['sl7_gallery_image']->aid = $aid;
      break;
    case 'edit':
      $form_state['sl7_gallery_image'] = is_object($image) ? $image : sl7_gallery_image_load($image);
      drupal_set_title('Редактирование изображения');
      break;
    default:
      $form_state['sl7_gallery_image'] = $image;
      break;
  }

  form_load_include($form_state, 'inc', 'sl7_gallery_image', 'includes/sl7_gallery_image.ui');

  $form_state['build_info']['args'] = array($op, $aid, $image);

  return drupal_build_form('sl7_gallery_image_edit_form', $form_state);
}