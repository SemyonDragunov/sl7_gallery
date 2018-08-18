<?php

/**
 * @file
 * sl7_gallery_album API.
 */

function sl7_gallery_album_access($op, $album = NULL, $account = NULL) {
  $rights = &drupal_static(__FUNCTION__, array());

  if (!empty($account)) {
    $account = is_object($account) ? $account : user_load($account);
  }
  else {
    $account = $GLOBALS['user'];
  }

  if ($account->uid == 1) {
    return TRUE;
  }

  if (!$album) {
    if ($op == 'create' && user_access("sl7_gallery_album $op", $account)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  if ($album) {
    $album = is_object($album) ? $album : sl7_gallery_album_load($album);
    $aid = $album->aid;
  }

  if (isset($rights[$account->uid][$aid][$op])) {
    return $rights[$account->uid][$aid][$op];
  }

  if (user_access('administer sl7_gallery', $account)) {
    $rights[$account->uid][$aid][$op] = TRUE;
    return TRUE;
  }

  if ($album && in_array($op, array('edit', 'delete'), TRUE)) {
    if (user_access("sl7_gallery_album any $op", $account)) {
      $rights[$account->uid][$aid][$op] = TRUE;
      return TRUE;
    }
    if (user_access("sl7_gallery_album own $op", $account) && $album->uid == $account->uid) {
      $rights[$account->uid][$aid][$op] = TRUE;
      return TRUE;
    }
  }

  if ($op == 'view' && $aid && user_access("sl7_gallery $op", $account)) {
    $rights[$account->uid][$aid][$op] = TRUE;
    return TRUE;
  }

  return FALSE;
}

function sl7_gallery_album_create($values = array()) {
  return entity_get_controller('sl7_gallery_album')->create($values);
}

function sl7_gallery_album_load($aid, $reset = FALSE) {
  $album = sl7_gallery_album_load_multiple(array($aid), array(), $reset);
  return reset($album);
}

function sl7_gallery_album_load_multiple($aids = array(), $conditions = array(), $reset = FALSE) {
  $aids = ($aids) ? $aids : FALSE;
  return entity_load('sl7_gallery_album', $aids, $conditions, $reset);
}

function sl7_gallery_album_delete(SL7GalleryAlbum $album) {
  $album->delete();
}

function sl7_gallery_album_delete_multiple(array $aids) {
  entity_get_controller('sl7_gallery_album')->delete($aids);
}

function sl7_gallery_album_save(SL7GalleryAlbum $album) {
  return entity_save('sl7_gallery_album', $album);
}

function sl7_gallery_album_uri(SL7GalleryAlbum $album) {
  return array(
    'path' => $album->realPath(),
  );
}

function sl7_gallery_album_title(SL7GalleryAlbum $album) {
  return $album->label;
}

/**
 * Получить все изображения определенного альбома.
 *
 * @param $album
 *  ID альбома или объект.
 * @return mixed
 *  FALSE при пустом альбоме или массив объектов SL7GalleryImage.
 */
function sl7_gallery_album_get_images($album) {
  $album = is_object($album) ? $album : sl7_gallery_album_load($album);

  $iids = db_select('sl7_gallery_image', 'i')
    ->condition('i.aid', $album->aid)
    ->fields('i', array('iid'))
    ->execute()
    ->fetchAssoc();

  if (!$iids) {
    return FALSE;
  }

  return sl7_gallery_image_load_multiple($iids);
}

/**
 * Получить обложку альбома.
 *
 * @param $album
 *  ID of object album.
 * @return mixed
 *  SL7GalleryImage object or URL "no-thumb".
 */
function sl7_gallery_album_get_cover($album) {
  $album = is_object($album) ? $album : sl7_gallery_album_load($album);

  $iid = db_select('sl7_gallery_image', 'i')
    ->condition('i.aid', $album->aid)
    ->condition('i.cover', 1)
    ->fields('i', array('iid'))
    ->execute()
    ->fetchField();

  // Если нет явно указанной обложки, используем созданное позднее изображение.
  if (!$iid) {
    $query = db_select('sl7_gallery_image', 'i')
      ->condition('i.aid', $album->aid);
    $query->addExpression('MAX(created)');
    $max = $query->execute()->fetchField();
    $iid = $query->condition('i.created', $max, '=')
      ->fields('i', array('iid'))
      ->execute()
      ->fetchField();
  }

  // Если альбом пустой
  if (!$iid) {
    return drupal_get_path('module', 'sl7_gallery') . '/img/no-thumb.png';
  }

  return sl7_gallery_image_load($iid);
}

/**
 * Установить обложку для альбома.
 *
 * @param $image
 * @return bool
 */
function sl7_gallery_album_set_cover($image) {
  $image = is_object($image) ? $image : sl7_gallery_image_load($image);

  $transaction = db_transaction();

  $old_cover_iids = db_select('sl7_gallery_image', 'i')
    ->condition('i.aid', $image->aid)
    ->condition('i.cover', 1)
    ->fields('i', array('iid'))
    ->execute()
    ->fetchAssoc();

  try {
    if ($old_cover_iids) {
      foreach ($old_cover_iids as $old_cover_iid) {
        db_update('sl7_gallery_image')
          ->condition('iid', $old_cover_iid)
          ->fields(array('cover' => 0))
          ->execute();
      }
    }

    db_update('sl7_gallery_image')
      ->condition('iid', $image->iid)
      ->fields(array('cover' => 0))
      ->execute();

    return TRUE;
  }
  catch (Exception $e) {
    $transaction->rollback();
     watchdog_exception('error', $e);
  }
}

/**
 * Metadata fields.
 */

function sl7_gallery_metadata_sl7_gallery_album_cover_url($entity, array $options, $name, $entity_type) {
  global $base_url;

  if (sl7_gallery_album_get_cover($entity)) {
    return image_style_url('sl7_gallery_album', sl7_gallery_image_load(sl7_gallery_album_get_cover($entity))->sl7_gallery_image['und'][0]['uri']);
  }
  else {
    return $base_url . '/' . drupal_get_path('module', 'sl7_gallery') . '/img/no-thumb.png';
  }
}