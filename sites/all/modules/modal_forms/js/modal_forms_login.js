(function ($) {

Drupal.behaviors.initModalFormsLogin = {
  attach: function (context, settings) {
    $("a[href*='/user/login'], a[href*='?q=user/login']", context).once('init-modal-forms-login', function () {
      this.href = this.href.replace(/user\/login/,'modal_forms/nojs/login');
    }).addClass('ctools-use-modal ctools-modal-modal-popup-small');
/* ADDED FOCUS TO FIRST ELEMENT ON MODAL TOOLS FORM -- https://drupal.org/node/1440336  */
    $("#edit-name").focus();
  }
};

})(jQuery);
