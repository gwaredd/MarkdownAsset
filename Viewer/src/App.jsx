import { useState, useEffect } from 'react'
import { useTheme } from '@mui/material/styles'
import Box from '@mui/material/Box'
import Fab from '@mui/material/Fab'
import Editor from 'react-simple-code-editor'

import {
  IconTextPlus,
  IconFileText
} from '@tabler/icons-react'

//-----------------------------------------------------------------------------
// highlight.js

import hljs from 'highlight.js'
import javascript from 'highlight.js/lib/languages/javascript'
import markdown from 'highlight.js/lib/languages/markdown'

hljs.registerLanguage( 'markdown', markdown )
hljs.registerLanguage( 'javascript', javascript )
hljs.registerLanguage( 'js', javascript )

/**/
import './AppDark.css'
import 'highlight.js/styles/base16/monokai.css'
/*/
import './AppLight.css'
import 'highlight.js/styles/github.css'
/**/

//-----------------------------------------------------------------------------

import markdownit from 'markdown-it'
import md_tasklists from 'markdown-it-task-lists'
import md_video from '@vrcd-community/markdown-it-video'
import md_diagrams from 'markdown-it-textual-uml'
import md_math from 'markdown-it-math'
import md_highlight from 'markdown-it-highlightjs'
import md_anchors  from 'markdown-it-anchor'
import md_toc  from 'markdown-it-table-of-contents'
import md_replace_link from 'markdown-it-replace-link'

import mermaid from 'mermaid'

mermaid.initialize({
  startOnLoad: true,
  theme      : 'forest'
})

const opts_video = {
  youtube: { width: 640, height: 390 },
  vimeo  : { width: 500, height: 281 },
  vine   : { width: 600, height: 600, embed: 'simple' },
  prezi  : { width: 550, height: 400 }
}

const opts_highlight = {
  auto          : true,
  hljs          : hljs,
  code          : true,
  inline        : true,
  ignoreIllegals: true,
}

const opts_replace_link = {
  processHTML: true,
  replaceLink: (link, env) => link.startsWith('http') ? `javascript:window.ue.markdownbinding.openurl('${link}')` : link
}

const md_opts = {
  html      : true,
  linkify   : true,
  typography: true
}

const md = markdownit(md_opts)
  .use( md_highlight, opts_highlight )
  .use( md_tasklists, { enabled: true } )
  .use( md_video, opts_video )
  .use( md_math )
  .use( md_diagrams )
  .use( md_anchors.default )
  .use( md_toc )
  .use( md_replace_link, opts_replace_link )

//-----------------------------------------------------------------------------

const Edit = (props) => {

  const {code, setCode} = props
  const theme = useTheme()

  return (
    <Editor
      value         = {code}
      onValueChange = {setCode}
      highlight     = {code => hljs.highlight(code, {language: 'markdown', ignoreIllegals: true }).value}
      padding       = {theme.spacing(3)}
      autoFocus     = {true}
      style         = {{
        fontFamily: '"Fira code", "Fira Mono", monospace',
        fontSize  : 12,
        width     : '100vw',
        minHeight : '100vh',
      }}
    />
  )
}

//-----------------------------------------------------------------------------

const View = (props) => {

  const theme = useTheme()
  const {code} = props

  return (
    <main
      style={{
        width     : '100vw',
        minHeight : '100vh',
        padding   : theme.spacing(3),
      }}
      dangerouslySetInnerHTML={{__html: md.render( code )}}
    />
  )
}

//-----------------------------------------------------------------------------

export default function App() {

  const [open, setOpen] = useState( false )
  const [code, setCode] = useState( '' )

  useEffect(() => {
    if( window.ue && window.ue.markdownbinding ) {
      window.ue.markdownbinding.gettext().then( (text) => setCode(text) )
    }
  },[])

  const onToggle = () => {
    setOpen(!open)
  }

  const onUpdateCode = (code) => {
    if( window.ue && window.ue.markdownbinding ) {
      window.ue.markdownbinding.settext( code )
    }
    setCode(code)
  }

  return (
    <Box>
      <Fab
        style={{position:'fixed',top:'1.5em',right:'1.5em'}}
        onClick={onToggle}
      >
        { open ? <IconFileText/> : <IconTextPlus/> }
      </Fab>
      { open ? <Edit code={code} setCode={onUpdateCode}/> : <View code={code}/> }
    </Box>
  )
}
