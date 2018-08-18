<?php

/**
 * @file
 * sl7_gallery_image API.
 */

function sl7_gallery_image_access($op, $image = NULL, $account = NULL) {
  $rights = &drupal_static(__FUNCTION__, array());

  if (!empty($account)) {
    $account = is_object($account) ? $account : user_load($account);
  }
  else {
    $account = $GLOBALS['user'];
  }

  // User #1 has all privileges.
  if ($account->uid == 1) {
    return TRUE;
  }

  if (!$image) {
    if ($op == 'create' && user_access("sl7_gallery_image $op", $account)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  if ($image) {
    $image = is_object($image) ? $image : sl7_gallery_image_load($image);
    $iid = $image->iid;
  }

  if (isset($rights[$account->uid][$iid][$op])) {
    return $rights[$account->uid][$iid][$op];
  }

  if (user_access('administer sl7_gallery', $account)) {
    $rights[$account->uid][$iid][$op] = TRUE;
    return TRUE;
  }

  if ($image && in_array($op, array('edit', 'delete'))) {
    if (user_access("sl7_gallery_image any $op", $account)) {
      $rights[$account->uid][$iid][$op] = TRUE;
      return TRUE;
    }
    if (user_access("sl7_gallery_image own $op", $account) && $image->uid == $account->uid) {
      $rights[$account->uid][$iid][$op] = TRUE;
      return TRUE;
    }
  }

  if ($op == 'view' && $iid && user_access("sl7_gallery $op", $account)) {
    $rights[$account->uid][$iid][$op] = TRUE;
    return TRUE;
  }

  return FALSE;
}

function sl7_gallery_image_create($values = array()) {
  return entity_get_controller('sl7_gallery_image')->create($values);
}

function sl7_gallery_image_load($iids, $reset = FALSE) {
  $image = sl7_gallery_image_load_multiple(array($iids), array(), $reset);
  return reset($image);
}

function sl7_gallery_image_load_multiple($iids = array(), $conditions = array(), $reset = FALSE) {
  $iids = ($iids) ? $iids : FALSE;
  return entity_load('sl7_gallery_image', $iids, $conditions, $reset);
}

function sl7_gallery_image_delete(SL7GalleryImage $image) {
  $image->delete();
}

function sl7_gallery_image_delete_multiple(array $iids) {
  entity_get_controller('sl7_gallery_image')->delete($iids);
}

function sl7_gallery_image_save(SL7GalleryImage $image) {
  return entity_save('sl7_gallery_image', $image);
}

function sl7_gallery_image_uri(SL7GalleryImage $image) {
  return array(
    'path' => $image->realPath(),
  );
}

function sl7_gallery_image_title(SL7GalleryImage $image) {
  if (!empty($image->sl7_gallery_body['und'][0]['value'])) {
    return truncate_utf8($image->sl7_gallery_body['und'][0]['value'], 30);
  }
  else {
    return 'Изображение';
  }
}