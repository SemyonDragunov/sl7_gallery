<div class="<?php print $classes; ?>"<?php print $attributes; ?> itemscope itemtype="http://schema.org/ImageGallery">

  <?php print render($title_prefix); ?>

  <a href="<?php print $url; ?>">

    <figure>

      <img id="img" src="<?php print $cover; ?>" />

      <figcaption itemprop="caption description">
        <span><?php print $title; ?></span>
        <?php if (isset($element->sl7_gallery_body['und'][0])): ?>
          <p><?php print truncate_utf8(check_plain(strip_tags($element->sl7_gallery_body['und'][0]['value'])), 100, TRUE, TRUE); ?></p>
        <?php endif; ?>
      </figcaption>

    </figure>

  </a>

  <?php print render($title_suffix); ?>

</div>
